
# 🖥️ VLDWM Display Server

**VLDWM** (Velo Desktop Window Manager) is a custom display server built entirely from scratch—**no X11, no Wayland, no mercy.**  
It’s designed for performance, extensibility, and full control over the rendering pipeline.

---

## ✨ Features

- 🚀 **Direct Hardware Access** – Uses **DRM/KMS** for native display output
- 🎯 **Modern Input Handling** – Powered by **libinput** (keyboard, mouse, touchpad)
- ⚙️ **Hardware-accelerated Compositing** – Uses **EGL + OpenGL ES**
- 🪶 **Lightweight by Design** – No bloat, just minimal clean dependencies
- 🧩 **Modular Architecture** – Easy to maintain, extend, and embed

---

## 🧱 Architecture

VLDWM is structured into clean, modular layers:

- **Core Server** – Handles the event loop, session, and client lifecycle
- **Display Backend** – Interfaces directly with DRM/KMS
- **Input Subsystem** – Reads input events via libinput
- **Compositor** – Manages surfaces and renders them to screen
- **IPC Layer** – Handles communication with external clients (e.g., toolkits, apps)

---

## 🔧 Building

### 🧰 Dependencies

#### Ubuntu/Debian:
```bash
sudo apt install build-essential cmake pkg-config git \
  libdrm-dev libgbm-dev libegl1-mesa-dev libgl1-mesa-dev \
  libinput-dev libxkbcommon-dev libudev-dev libsystemd-dev libpixman-1-dev
````

#### Fedora/RHEL/CentOS:

```bash
sudo dnf install gcc cmake pkgconfig git \
  libdrm-devel mesa-libgbm-devel mesa-libEGL-devel mesa-libGL-devel \
  libinput-devel libxkbcommon-devel systemd-devel pixman-devel
```

#### Arch Linux:

```bash
sudo pacman -S base-devel cmake pkgconf git \
  libdrm mesa libinput libxkbcommon systemd pixman
```

---

### 🏗️ Compilation

```bash
# Clone and enter the project directory
git clone https://github.com/your-username/vldwm.git
cd vldwm

# Build (release mode)
make release

# Optionally install system-wide
sudo make install
```

---

## ⚙️ Development Workflow

### Debug / Dev Mode

```bash
make debug
make test     # Run test suite
make docs     # Generate documentation
```

### Run the Server

```bash
# Requires root for DRM access
sudo ./build/vldwm
sudo ./build/vldwm --debug
```

### Example Clients

```bash
./build/simple_client
./build/window_example
```

---

## ⚙️ Configuration

VLDWM reads configuration from the following locations:

* `~/.config/vldwm/config` (user override)
* `/etc/vldwm/config` (global default)

#### Example config:

```
# Display settings
output.resolution = 1920x1080
output.refresh_rate = 60

# Input behavior
input.mouse_acceleration = 1.0
input.keyboard_repeat_rate = 25
input.keyboard_repeat_delay = 250

# Compositor options
compositor.vsync = true
compositor.max_fps = 60
```

---

## 🧩 Project Layout

```
vldwm/
├── src/
│   ├── core/          # Server loop & session manager
│   ├── renderer/      # DRM/KMS + OpenGL rendering
│   ├── input/         # Input event handling
│   ├── compositor/    # Surface management & composition
│   ├── ipc/           # Communication between client <-> server
│   └── utils/         # Internal helpers
├── include/           # Public headers
├── examples/          # Demo clients
├── docs/              # Project documentation
└── tests/             # Test suite
```

---

## 🧪 Testing

```bash
make test
./build/tests/test_display
```

---

## 🚀 Performance-Oriented

* 🧠 **Zero-copy rendering** for maximum throughput
* 🧵 **Minimal context switching** = faster event loops
* 🎨 **Hardware-accelerated drawing** using OpenGL ES
* 🪄 **Lean memory footprint** and optimized resource use

---

## 🐛 Debugging

### Common Issues

| Symptom           | Fix                                               |
| ----------------- | ------------------------------------------------- |
| Permission denied | Add yourself to `video` group or run with `sudo`  |
| No display found  | Ensure DRM/KMS device is available (`/dev/dri/*`) |
| Input not working | Check `libinput` access or udev permissions       |

### Handy Tools

```bash
ls -l /dev/dri/        # List GPU devices
sudo evtest            # Monitor input devices
eglinfo                # Inspect EGL capabilities
```

---

## 🤝 Contributing

1. Fork this repo and clone it
2. Create a new feature branch
3. Commit clean, tested code with docs
4. Submit a pull request 🛠️

### Code Style

* 4-space indentation
* C99 standard
* Document all public APIs
* `make format` before pushing

---

## 📜 License

BSD-3 — see [LICENSE](./LICENSE)

---

## 🙏 Acknowledgments

* DRM/KMS & libinput devs for incredible APIs
* Mesa, Pixman, and EGL maintainers
* Linux kernel community
* Anyone who dares write a display server from scratch 🫡

---

## 🛣️ Roadmap

* [ ] Compositor polish (buffer management, vsync)
* [ ] Define client protocol (XDG-like or custom)
* [ ] Window decoration manager
* [ ] Multi-monitor / multi-GPU support
* [ ] Hotplug support
* [ ] Integrated logging + metrics
* [ ] Animations engine (VeloUi integration)
* [ ] Documentation site (`docs.vldwm.dev` maybe?)

---

> ⚡ *VLDWM is not a replacement for X or Wayland — it’s a rebellion.*
> A clean slate for the modern age of desktop computing.

