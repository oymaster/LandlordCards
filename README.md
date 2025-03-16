## 项目结构

```bash
├── client/                 # 客户端代码
│   ├── card/               # 扑克牌相关模块
│   ├── control/            # 游戏控制逻辑
│   ├── player/             # 玩家相关模块
│   ├── strategy/           # 游戏策略模块
│   ├── thread/             # 线程模块
│   ├── windows/            # 窗口界面模块
│   ├── tcp/                # 网络通信模块
│   ├── serialize/          # 序列化模块
│   ├── crypto/             # 加密模块
│   └── res.qrc             # 资源文件
├── server/                 # 服务器代码
│   ├── common/             # 公共模块
│   ├── http/               # HTTP 服务模块
│   ├── reactor/            # Reactor 模式实现
│   ├── tcp/                # TCP 通信模块
│   ├── thread/             # 线程池模块
│   ├── test/               # 测试模块
│   ├── crypto/             # 加密模块
│   ├── serialize/          # 序列化模块
│   ├── game/               # 游戏核心逻辑
│   ├── database/           # 数据库访问模块
│   └── main.cpp            # 服务器入口文件
└── README.md               # 项目说明文件
```

### **环境要求**

* **CMake ≥ 3.16**
* **C++17** 支持的编译器（如 GCC、Clang、MSVC）
* **Qt5 或 Qt6**（客户端需要）
* **OpenSSL**
* **Protobuf**（客户端需要）
* **MySQL、Redis、JSON（服务器需要）**

------

### 如何运行：

### 客户端：

以**Qt Creator**为例：**打开 Qt Creator**

1. **导入项目**

* 选择 **"文件" → "打开文件或项目"**。
* 选择 `CMakeLists.txt` 文件。
* Qt Creator 会自动解析 CMake 文件并生成项目配置。

2. **配置 CMake 工具链**
    在 **"项目" → "构建与运行"** 里配置：

* **构建套件**：选择安装的 Qt 版本（如 `Qt 6.5.2 MSVC 64-bit`）。
* **CMake 生成器**：选择 `Ninja` 或 `Visual Studio`（推荐 Ninja）。
* **编译器**：选择与 Qt 兼容的 C++ 编译器（如 MSVC 或 MinGW）。

3. **配置 OpenSSL 和 Protobuf 的路径**
    去cmakelist中修改自己对应的Protobuf、OpenSSL路径。

4. **构建项目**

* 在 Qt Creator 左侧面板中选择 **"构建项目"**。
* 如果出现 OpenSSL 或 Protobuf 的未找到错误，检查路径配置是否正确。

5. **运行客户端**

* 在 Qt Creator 中点击 **"运行"** 按钮。

### 服务端：

1. 安装上述服务器依赖库（以 Ubuntu 为例）：

2. 构建项目：

   ```bash
   mkdir build
   cd build
   cmake ..
   make -j$(nproc)
   ```

3. 运行服务器：

   ```bash
   ./bin/server
   ```

------

## 功能特性

### 🃏 客户端

*  基于 Qt 的跨平台 GUI
*  扑克牌管理与显示
*  多人在线匹配
*  游戏逻辑与策略

### 🌐 服务器

*  Reactor 模式实现的高性能网络通信
*  Redis/MySQL 结合的数据持久化
*  基于 SSL 的安全加密通信
*  支持高并发的线程池

------

## 待改进

*  提示功能和房间优化
*  完善异常处理与日志记录
