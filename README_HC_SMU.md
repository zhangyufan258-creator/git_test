# 四通道HC-SMU校准板控制系统说明文档

## 一、项目概述

本项目为四通道HC-SMU校准板的完整控制系统，实现电流源/电压源的测压测流功能。系统基于STM32F1微控制器，通过Modbus RTU协议进行远程控制。

**核心功能：**
- 四通道独立切换控制
- 六档负载电阻精确选择
- 电流源/电压源模式切换
- MOS管开关控制
- 485通信接口支持

---

## 二、硬件参数与引脚配置

### 2.1 核心硬件参数

| 参数类型 | 规格 |
|---------|------|
| 电压测量/输出范围 | 0~5V |
| 电流测量/输出范围 | 0~1A |
| 通信接口 | RS485 (DB9接口) |
| 供电电压 | 15V |
| 烧录接口 | JTAG |
| 微控制器 | STM32F103 |

### 2.2 引脚定义表

#### 档位选择引脚 (7个引脚控制6档负载)

| 引脚名称 | GPIO定义 | 端口/引脚 | 功能说明 |
|---------|---------|----------|---------|
| RES     | RELAY_RES_Pin | PA1 | 档位选择位6 |
| RES01   | RELAY_RES01_Pin | PC1 | 档位选择位5 |
| RES02   | RELAY_RES02_Pin | PC2 | 档位选择位4 |
| RES03   | RELAY_RES03_Pin | PC3 | 档位选择位3 |
| RES04   | RELAY_RES04_Pin | PC4 | 档位选择位2 |
| RES05   | RELAY_RES05_Pin | PC5 | 档位选择位1 |
| RES06   | RELAY_RES06_Pin | PB0 | 档位选择位0 |

#### 通道选择引脚

| 引脚名称 | GPIO定义 | 端口/引脚 | 功能说明 |
|---------|---------|----------|---------|
| CH01    | RELAY_CH01_Pin | PA4 | 通道1选择 |
| CH02    | RELAY_CH02_Pin | PA5 | 通道2选择 |
| CH03    | RELAY_CH03_Pin | PA6 | 通道3选择 |
| CH04    | RELAY_CH04_Pin | PA7 | 通道4选择 |

#### 控制引脚

| 引脚名称 | GPIO定义 | 端口/引脚 | 功能说明 |
|---------|---------|----------|---------|
| MOS     | MOS_Pin | PA3 | MOS管控制 (高电平=HF正常工作, 低电平=HF停止) |
| OS      | RELAY_OS_Pin | PA2 | 输出短接到地控制 |

### 2.3 档位配置表

| 档位编号 | 电流范围 | 负载电阻 | 功率 | 引脚配置 [RES~RES06] | 枚举值 |
|---------|---------|---------|------|---------------------|--------|
| 档位1 | 1A | 5Ω | 5W | 0000001 | GEAR_1A (0) |
| 档位2 | 200mA | 25Ω | 1W | 0000010 | GEAR_200MA (1) |
| 档位3 | 20mA | 250Ω | 100mW | 0000100 | GEAR_20MA (2) |
| 档位4 | 2mA | 2.5KΩ | 10mW | 0001000 | GEAR_2MA (3) |
| 档位5 | 200uA | 25KΩ | 1mW | 0010000 | GEAR_200UA (4) |
| 档位6 | 20uA | 250KΩ | 0.1mW | 0100000 | GEAR_20UA (5) |

---

## 三、代码模块结构

### 3.1 文件组织架构

```
Four_CH_CalibrationBoard/
├── User/
│   ├── Inc/
│   │   ├── hc_smu_control.h      # HC-SMU控制模块头文件 (新增)
│   │   └── command.h              # 命令处理模块头文件 (已完善)
│   └── Src/
│       ├── hc_smu_control.c      # HC-SMU控制模块实现 (新增)
│       └── command.c              # 命令处理模块实现 (已完善)
├── Core/
│   ├── Src/
│   │   ├── main.c                # 主程序 (已集成)
│   │   └── gpio.c                # GPIO初始化
│   └── Inc/
│       └── main.h                # 主程序头文件
└── User/Lib/FreeModbus/          # Modbus协议栈
```

### 3.2 模块功能说明

#### 3.2.1 HC-SMU控制模块 (hc_smu_control.c/h)

**功能职责：** 硬件底层控制的抽象层，提供标准化的API接口

**核心功能模块：**

1. **初始化模块**
   - `HC_SMU_Init()`: 系统初始化，设置所有引脚为安全状态
   - `HC_SMU_Reset()`: 系统复位恢复

2. **MOS管控制模块**
   - `HC_SMU_SetMOSState()`: 设置MOS管开/关状态
   - `HC_SMU_GetMOSState()`: 获取当前MOS管状态

3. **通道切换模块**
   - `HC_SMU_SelectChannel()`: 切换到指定通道
   - `HC_SMU_CloseAllChannels()`: 关闭所有通道
   - `HC_SMU_GetCurrentChannel()`: 获取当前通道

4. **模式配置模块**
   - `HC_SMU_SetMeasureMode()`: 设置测量模式(电流源/电压源)
   - `HC_SMU_GetMeasureMode()`: 获取当前测量模式

5. **档位选择模块**
   - `HC_SMU_SetResistorGear()`: 设置负载电阻档位
   - `HC_SMU_GetResistorGear()`: 获取当前档位
   - `HC_SMU_DisableAllResistors()`: 关闭所有负载

6. **综合控制模块**
   - `HC_SMU_ConfigMeasure()`: 一次性配置完整测量参数

**关键设计特点：**
- 所有GPIO操作都经过封装，便于移植和维护
- 切换操作自动包含安全延时，防止继电器冲突
- 状态机管理，实时跟踪系统状态
- 档位配置表驱动，易于扩展新档位

#### 3.2.2 命令处理模块 (command.c/h)

**功能职责：** Modbus寄存器解析与业务逻辑处理

**核心功能：**
- `CMD_ProcessTask()`: 主任务函数，检测寄存器变化并执行控制
- `CMD_IsSystemUnlocked()`: 系统锁定状态检查

**处理流程：**
1. 检查系统解锁状态 (需要写入0xDEAD, 0xBEEF到锁定寄存器)
2. 监测各功能寄存器变化
3. 调用HC-SMU控制模块执行硬件操作
4. 缓存寄存器状态用于下次比较

#### 3.2.3 主程序模块 (main.c)

**初始化流程：**
```c
1. HAL_Init()               // HAL库初始化
2. SystemClock_Config()     // 系统时钟配置
3. MX_GPIO_Init()           // GPIO初始化
4. MX_USART1_UART_Init()    // 串口初始化 (485通信)
5. eMBInit() + eMBEnable()  // Modbus初始化
6. init_DATA_Struct()       // 数据结构初始化
7. HC_SMU_Init()            // HC-SMU模块初始化
```

**主循环：**
```c
while(1) {
    eMBPoll();           // Modbus协议处理
    CMD_ProcessTask();   // 命令处理任务
}
```

---

## 四、Modbus寄存器映射表

### 4.1 保持寄存器 (Holding Register) - 可读写

| 寄存器地址 | 名称 | 功能说明 | 有效值范围 |
|-----------|------|---------|-----------|
| 0 | SYS_Lock0 | 系统锁定寄存器0 | 0xDEAD (解锁) |
| 1 | SYS_Lock1 | 系统锁定寄存器1 | 0xBEEF (解锁) |
| 2 | SYS_Switch_LoadRes | 负载电阻档位选择 | 0~5 (对应6档), >5=关闭所有 |
| 3 | SYS_Switch_channle | 通道选择 | 0~3 (通道1~4), >3=关闭所有 |
| 4 | SYS_Switch_IVSource | 电流/电压源模式 | 0=电流源, 1=电压源 |
| 5 | SYS_Control_Shortground | MOS管控制 | 0=关闭(HF停止), 1=开启(HF工作) |

### 4.2 输入寄存器 (Input Register) - 只读

| 寄存器地址 | 名称 | 功能说明 | 示例值 |
|-----------|------|---------|--------|
| 0 | Version_0 | 固件版本 - 年 | 0x2025 |
| 1 | Version_1 | 固件版本 - 月日 | 0x1220 |
| 2 | Version_2 | 固件版本 - 时分 | 0x1905 |

### 4.3 Modbus通信参数

| 参数 | 值 |
|-----|---|
| 从机地址 | 0x01 (可修改) |
| 波特率 | 115200 |
| 数据位 | 8 |
| 校验位 | Even (偶校验) |
| 停止位 | 1 |
| 模式 | RTU |

---

## 五、使用指南

### 5.1 系统初始化流程

上电后系统自动执行以下操作：
1. 关闭所有通道
2. 关闭所有负载电阻
3. MOS管设置为关闭状态
4. 默认模式设置为电压源模式

### 5.2 通过Modbus控制示例

#### 示例1: 测量通道1的电压源 (档位2, 200mA)

**操作步骤：**
```
步骤1: 解锁系统
  写寄存器0 = 0xDEAD
  写寄存器1 = 0xBEEF

步骤2: 设置为电压源模式
  写寄存器4 = 1

步骤3: 选择档位2 (200mA, 25Ω)
  写寄存器2 = 1

步骤4: 选择通道1
  写寄存器3 = 0

步骤5: 开启MOS管
  写寄存器5 = 1
```

#### 示例2: 测量通道3的电流源

**操作步骤：**
```
步骤1: 解锁系统
  写寄存器0 = 0xDEAD
  写寄存器1 = 0xBEEF

步骤2: 设置为电流源模式
  写寄存器4 = 0

步骤3: 选择通道3
  写寄存器3 = 2

步骤4: 开启MOS管
  写寄存器5 = 1
```

**注意：** 电流源模式下不需要设置负载档位，系统会自动关闭所有负载

### 5.3 直接调用API示例

如果需要在本地程序中直接控制硬件（不通过Modbus）：

```c
#include "hc_smu_control.h"

// 示例: 配置通道2, 电压源模式, 档位4(2mA), MOS开启
HC_SMU_Status_t status;

status = HC_SMU_ConfigMeasure(
    CHANNEL_2,           // 通道2
    MODE_VOLTAGE_SOURCE, // 电压源模式
    GEAR_2MA,            // 档位4 (2mA, 2.5KΩ)
    MOS_ON               // MOS管开启
);

if (status == HC_SMU_OK) {
    // 配置成功
}

// 或者分步配置
HC_SMU_SelectChannel(CHANNEL_2);
HC_SMU_SetMeasureMode(MODE_VOLTAGE_SOURCE);
HC_SMU_SetResistorGear(GEAR_2MA);
HC_SMU_SetMOSState(MOS_ON);
```

---

## 六、集成与编译指引

### 6.1 文件集成清单

**新增文件：**
- `User/Inc/hc_smu_control.h`
- `User/Src/hc_smu_control.c`

**修改文件：**
- `User/Inc/command.h` (添加函数声明)
- `User/Src/command.c` (添加业务逻辑)
- `Core/Src/main.c` (集成初始化和主循环调用)

### 6.2 编译配置

**Include路径确认：**
确保以下路径在编译器Include Path中：
- `User/Inc`
- `Core/Inc`
- `Drivers/STM32F1xx_HAL_Driver/Inc`
- `Drivers/CMSIS/Device/ST/STM32F1xx/Include`

**源文件添加：**
在IDE项目中添加以下源文件到编译列表：
- `User/Src/hc_smu_control.c`

### 6.3 可能的编译问题处理

#### 问题1: 找不到头文件

**解决方案：**
检查Include Path是否包含`User/Inc`目录

#### 问题2: GPIO引脚未定义错误

**解决方案：**
确保`main.h`中包含完整的GPIO引脚定义，参考本文档第2.2节

#### 问题3: Modbus协议栈链接错误

**解决方案：**
确保FreeModbus库的所有源文件都已添加到项目中

### 6.4 下载与调试

**烧录方式：**
1. JTAG接口烧录
2. 通过BOOT0/BOOT1跳线进入Bootloader模式

**调试建议：**
- 使用串口助手监控485通信数据
- 通过万用表测量继电器输出端验证控制逻辑
- 在关键函数处设置断点跟踪执行流程

---

## 七、安全注意事项

### 7.1 硬件安全

1. **通道切换延时：** 系统自动添加延时，避免继电器同时导通
2. **档位切换保护：** 切换前先关闭所有负载，防止冲击
3. **MOS管控制：** 配置过程中MOS保持关闭，配置完成后再开启
4. **上电状态：** 系统上电后所有输出默认关闭

### 7.2 软件安全

1. **系统锁定机制：** 必须先解锁才能执行控制命令
2. **参数边界检查：** 所有API都包含参数有效性验证
3. **错误返回码：** 每个函数都返回状态码，便于错误处理

### 7.3 操作建议

1. 切换模式前先关闭MOS管
2. 测量前确认档位与电流范围匹配
3. 避免频繁切换继电器，建议间隔>100ms
4. 定期检查继电器触点状态

---

## 八、扩展与二次开发

### 8.1 添加新档位

**步骤：**
1. 在`hc_smu_control.h`中添加新枚举值
2. 修改`RESISTOR_GEAR_COUNT`宏定义
3. 在`hc_smu_control.c`的`GEAR_CONFIG_TABLE`中添加引脚配置
4. 重新编译

### 8.2 扩展通道数量

**步骤：**
1. 修改`CHANNEL_COUNT`宏定义
2. 在`CHANNEL_PINS`数组中添加新通道引脚
3. 确保硬件GPIO已正确初始化

### 8.3 修改Modbus参数

**修改波特率：**
在`main.c`的`eMBInit()`函数调用处修改参数：
```c
eMBInit(MB_RTU, 1, 0x01, 9600, MB_PAR_EVEN);  // 改为9600波特率
```

**修改从机地址：**
```c
eMBInit(MB_RTU, 1, 0x02, 115200, MB_PAR_EVEN);  // 地址改为0x02
```

### 8.4 添加状态监控寄存器

可在`INPUT_REG`结构体中添加新字段，用于上报设备状态：

```c
typedef struct INPUT_REG {
    uint16_t Version_0;
    uint16_t Version_1;
    uint16_t Version_2;
    uint16_t Current_Channel;   // 新增: 当前通道
    uint16_t Current_Gear;      // 新增: 当前档位
    uint16_t Current_Mode;      // 新增: 当前模式
} INPUT_REG;
```

在`CMD_ProcessTask()`中更新这些寄存器值。

---

## 九、常见问题FAQ

**Q1: 为什么写入寄存器后没有反应？**
A: 检查系统是否已解锁（寄存器0和1必须写入0xDEAD和0xBEEF）

**Q2: 通道切换时出现异常？**
A: 系统已内置通道切换保护，如仍有问题，检查GPIO初始化是否正确

**Q3: 如何确认当前系统状态？**
A: 调用`HC_SMU_GetSystemState()`获取完整状态信息

**Q4: 电流源模式下是否需要设置档位？**
A: 不需要，电流源模式下系统会自动关闭所有负载电阻

**Q5: 如何验证485通信是否正常？**
A: 使用Modbus调试工具读取输入寄存器0~2，应返回固件版本号

**Q6: MOS管控制逻辑是高电平有效还是低电平有效？**
A: 高电平有效。写1(MOS_ON)使HF正常工作，写0(MOS_OFF)拉低HF至COM

---

## 十、技术支持

**代码生成信息：**
- 生成日期: 2026-01-03
- 生成工具: Claude Code
- 基础工程: STM32F103 HAL + FreeModbus

**联系与反馈：**
如遇到问题或需要技术支持，请记录：
1. 具体现象和错误信息
2. 操作步骤
3. Modbus寄存器读写记录
4. 编译器版本和配置

---

## 附录A: 快速参考卡

### Modbus快速操作表

| 操作 | 寄存器 | 写入值 |
|-----|-------|-------|
| 解锁系统 | 0, 1 | 0xDEAD, 0xBEEF |
| 选择通道1 | 3 | 0 |
| 选择通道2 | 3 | 1 |
| 选择通道3 | 3 | 2 |
| 选择通道4 | 3 | 3 |
| 关闭所有通道 | 3 | ≥4 |
| 电流源模式 | 4 | 0 |
| 电压源模式 | 4 | 1 |
| 档位1(1A) | 2 | 0 |
| 档位2(200mA) | 2 | 1 |
| 档位3(20mA) | 2 | 2 |
| 档位4(2mA) | 2 | 3 |
| 档位5(200uA) | 2 | 4 |
| 档位6(20uA) | 2 | 5 |
| MOS关闭 | 5 | 0 |
| MOS开启 | 5 | 1 |

### API快速查找

```c
// 初始化
HC_SMU_Init()
HC_SMU_Reset()

// MOS控制
HC_SMU_SetMOSState(MOS_ON/MOS_OFF)
HC_SMU_GetMOSState()

// 通道控制
HC_SMU_SelectChannel(CHANNEL_1~4)
HC_SMU_CloseAllChannels()
HC_SMU_GetCurrentChannel()

// 模式控制
HC_SMU_SetMeasureMode(MODE_CURRENT_SOURCE/MODE_VOLTAGE_SOURCE)
HC_SMU_GetMeasureMode()

// 档位控制
HC_SMU_SetResistorGear(GEAR_1A~GEAR_20UA)
HC_SMU_GetResistorGear()
HC_SMU_DisableAllResistors()

// 综合配置
HC_SMU_ConfigMeasure(channel, mode, gear, mos_state)
```

---

**文档版本:** v1.0
**最后更新:** 2026-01-03
