<img width="10%" height="10%" alt="638233980-2bcde239-ea03-4da4-bac8-44363f73b338" src="https://github.com/user-attachments/assets/f50ca6c6-3603-40e5-b584-7c5fb930d16c" />


# 元素周期表 · PenOS 小程序

一个运行在有道词典笔（PenOS / falcon 小程序框架）上的**元素周期表** miniapp，使用 Vue 开发，构建产物为 `.amr` 安装包。

## 功能特性

- **完整元素周期表**：118 个元素，18 列标准布局，镧系 / 锕系独立成行，按类别着色（碱金属、碱土金属、过渡金属、镧系、锕系、主族金属、类金属、非金属、卤素、稀有气体）
- **自适应屏幕**：词典笔屏幕 260×640 旋转 270° 后按 640×260 布局，宽度用 flex 百分比自适应，表格可滚动
- **主页缩放**：0.6× ~ 1.5× 缩放，格子高度与字号联动，缩放值持久保存
- **内置搜索 + 内置键盘**：设备无实体键盘，内置自制键盘（字母 / 数字双模式），支持按中文名拼音（如 `tie` → 铁）、元素符号、原子序数实时搜索
- **元素详情页**：符号、名称、拼音、原子量、电负性、价电子数、价电子排布式、电子排布、分类、周期、族、常温物态、来源
- **设置页**（设置自动保存，返回主页立即生效）：
  - 主页显示电负性
  - 主页显示相对原子质量
  - 主页显示价电子
  - 主页显示周期（表格左侧周期列）
  - 主页显示价电子排布式
- **图例弹层**：10 类元素分类颜色说明

## 目录结构

```
├── aiot-vue-cli/          # 构建工具（vendored，aiot-vue-cli@1.0.32）
├── jsapi/                 # 原生 C++ 库（跨编译工具链，构建时下载）
├── tools/build.sh         # 构建脚本
├── ui/
│   ├── assets/            # 随包分发的静态资源
│   ├── src/
│   │   ├── app.js         # 应用生命周期（模板基类，勿改）
│   │   ├── app.json       # 页面路由：index / page / settings
│   │   ├── base-page.js   # 页面基类（模板基类，勿改）
│   │   ├── components/
│   │   │   └── KeyBoard.vue    # 内置键盘
│   │   ├── data/
│   │   │   └── elements.js     # 118 个元素完整数据
│   │   ├── pages/
│   │   │   ├── index/index.vue     # 主页：周期表
│   │   │   ├── page/page.vue       # 元素详情
│   │   │   └── settings/settings.vue  # 设置
│   │   └── styles/        # 共享样式
│   └── package.json       # 应用元信息（appid、版本、图标）
├── icon1.png / icon2.png  # 应用图标源文件
└── .github/workflows/     # CI：AArch64（P5）自动构建 .amr
```

## 截图
<img width="800" height="254" alt="capture_20260816_114744" src="https://github.com/user-attachments/assets/7791a34f-d99a-451b-885b-9079c59101f9" />

## 构建

### 本地构建（仅 UI 部分）

```bash
# 1. 安装依赖（pnpm 10+，Node 18+）
pnpm install

# 2. 构建（生成 ui/8001749644971193.0_0_2.amr）
pnpm -C ui build
```

> 完整设备构建需在 `jsapi/toolchains/` 放置交叉编译工具链并执行 `./tools/build.sh`（Linux 环境），详见 CI 配置。

### CI / GitHub Actions

push 到 `main` 后自动构建 **AArch64（P5）** 设备包（`aarch64--glibc` 工具链），产物上传为 workflow artifacts。

## 开发注意事项

- 样式约束（falcon-styler 校验）：
  - **只支持单类名选择器**，禁止 `.a .b` 这类后代选择器
  - `border-radius` 只支持单值（数字或 px）
  - 字号不宜过小；内容放不下时用 `scroller` 滚动
- 模板绑定方法时需**带括号调用**（如 `:style="cellRowStyle()"`），否则编译后绑定的是函数对象本身
- 页面跳转使用 `$falcon.navTo('page', { num: 26 })`，返回用 `this.$page.finish()`
- 设置持久化使用 `$falcon.jsapi.storage`，主页在 `onShow()` 时重新读取

## 致谢

- 基于https://github.com/penosext/miniapp-template的模板

## 许可证

[GNU General Public License v3.0](LICENSE)
