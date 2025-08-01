# Ienium Utils
A collection of lightweight header-only C++ utilities. These libraries are designed to be simple and easy to integrate into your projects.

Currently the only available library is Ienium Vault.

## 🔐 Vault 
Securely stores OAuth tokens, API keys and other sensitive data using the Windows Credential Manager.
### How to use
```cpp
#include "vault/ieniumvault.hpp"

IeniumVault vault (L"YourApp");
vault.StoreCredentials(L"ConnectionAPIKey", L"your_api_key_here");
auto my_api_key = vault.GetCredentials(L"ConnectionAPIKey");
vault.DeleteCredentials ("ConnectionAPIKey");
```

## 🚀 Quick Start

### 1. Add to Your Project

**Option A: Copy Headers**
Simply copy the headers you need into your project.

**Option B: Git Submodule**
```bash
git submodule add https://github.com/Ienium/IeniumUtils.git external/ienium-utils
```

### 2. Include in CMake

```cmake
# If using submodule
target_include_directories(your_target PRIVATE external/ienium-utils)

# Link required Windows libraries
target_link_libraries(your_target advapi32)
```

## 📋 Requirements

- **Platform:** Windows 7+ 
- **Compiler:** MSVC 2019+, MinGW-w64, or Clang
- **C++ Standard:** C++17 or later

## 🏗️ Architecture

All utilities are **header-only** for maximum convenience:
- ✅ No compilation required
- ✅ Easy integration  
- ✅ Full compiler optimization
- ✅ Cross-project portability

## 📄 License

This project is licensed under the MIT License - see the [LICENSE](LICENSE) file for details.