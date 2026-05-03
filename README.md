# FemboyRobin
CS2 External Cheat

---

## Proje Yapısı

```
FemboyRobin/
│
├── FemboyRobin/                    # ★ ANA PROJE - External C++ Cheat
│   │
│   └── project/
│       │
│       ├── core/                   # Çekirdek sistemler
│       │   ├── features/           # Özellikler (ESP, Aimbot, vs)
│       │   │   └── impl/
│       │   │       ├── esp/        # ESP sistemi
│       │   │       │   ├── player.cpp    # Oyuncu ESP (skeleton, box, hitbox)
│       │   │       │   ├── item.cpp      # Silah/Item ESP
│       │   │       │   └── projectile.cpp # Bomba/Granat ESP
│       │   │       └── combat/     # Combat sistemi
│       │   │           ├── legit.cpp     # Legit aimbot
│       │   │           └── shared.cpp   # Ortak combat fonksiyonlari
│       │   │
│       │   ├── menu/               # Menü sistemi
│       │   │   ├── menu.cpp
│       │   │   └── menu.hpp
│       │   │
│       │   ├── render/             # Render sistemi (DirectX)
│       │   │   ├── render.cpp      # Draw calls, text, shapes
│       │   │   └── render.hpp
│       │   │
│       │   ├── systems/            # Oyun verisi toplama sistemleri
│       │   │   ├── systems.hpp     # Sistem header
│       │   │   └── impl/
│       │   │       ├── bones.cpp        # Bone matrix okuma
│       │   │       ├── bounds.cpp       # Hitbox bounds hesaplama
│       │   │       ├── bvh.cpp          # Ray tracing / visibility
│       │   │       ├── collector.cpp    # Oyuncu/item toplama
│       │   │       ├── entities.cpp     # Entity list okuma
│       │   │       ├── hitboxes.cpp     # Hitbox verisi okuma
│       │   │       ├── local.cpp        # Local player verileri
│       │   │       ├── schemas.cpp      # Schema hash'ler
│       │   │       └── view.cpp         # View matrix, world to screen
│       │   │
│       │   └── threads/           # Thread yönetimi
│       │       └── threads.cpp
│       │
│       ├── external/              # Dis kütüphaneler
│       │   ├── zdraw/              # Özel draw kütüphanesi
│       │   │   ├── zdraw.cpp
│       │   │   ├── zdraw.hpp
│       │   │   ├── zui/           # UI sistemi
│       │   │   └── external/
│       │   │       ├── shaders/    # Vertex/Fragment shader'lar
│       │   │       └── freetype/  # Font rendering
│       │   └── poly2d.hpp        # 2D polygon helper
│       │
│       ├── resources/             # Kaynaklar
│       │   └── fonts/            # Font dosyaları (mochi, pixel7, pretzel)
│       │
│       ├── utilities/              # Yardımcı sistemler
│       │   ├── offsets/          # Offset tanımları
│       │   │   ├── offsets.hpp   # ★ ÖNEMLİ - Tüm memory offset'leri burada
│       │   │   └── offsets.cpp
│       │   ├── memory/           # Memory okuma/yazma
│       │   │   ├── memory.cpp
│       │   │   └── memory.hpp
│       │   ├── math/              # Matematik fonksiyonları
│       │   │   ├── math.cpp
│       │   │   └── math.hpp
│       │   ├── console/           # Konsol sistemi
│       │   ├── input/            # Input alma (keyboard, mouse)
│       │   ├── modules/          # MODULE handling (GetModuleBase)
│       │   ├── animation/        # Animation sistemleri
│       │   └── random.hpp        # Random helper'lar
│       │
│       ├── stdafx.hpp            # Precompiled header
│       ├── pch/                  # Precompiled header files
│       └── entry.cpp             # Entry point (DllMain benzeri)
│
├── output/                        # ★ OFFSET DUMP - Dışarıdan alınan offsetler
│   ├── client_dll.hpp            # client.dll tüm offsetler
│   ├── offsets.json              # Offset'ler JSON formatında
│   ├── offsets.hpp              # C++ header olarak offsetler
│   ├── buttons.hpp              # Tuş bağlama offsetleri
│   ├── interfaces.hpp           # Interface'ler (IVEngineClient, vs)
│   ├── engine2_dll.hpp          # engine2.dll offsetleri
│   ├── animationsystem_dll.hpp  # Animation system offsetleri
│   └── *.json, *.cs, *.rs, *.zig # Çeşitli dillerde output
│
├── int/                          # ★ İNTERNAL CHEAT - Internal C++ proje
│   ├── Ai cheat.sln              # Visual Studio solution
│   ├── Ai cheat.vcxproj          # Visual Studio project
│   ├── dllmain.cpp              # DLL entry point
│   ├── hooks/
│   │   └── present.cpp          # EndScene hook (render劫持)
│   ├── output/                  # Internal offset dump (güncel)
│   │   ├── client_dll.hpp       # Internal client.dll offsets
│   │   └── offsets.json         # Internal offsets
│   ├── imgui-master/            # ImGui kütüphanesi (menü için)
│   ├── minhook-master/          # MinHook (hook kütüphanesi)
│   └── Skinchanger/             # .NET Skin changer tool
│       ├── Core/
│       │   ├── Memory.cs        # Memory işlemleri
│       │   ├── Offsets.cs       # Offset tanımları
│       │   └── SkinChanger.cs   # Skin değiştirme logic
│       └── MainWindow.axaml     # UI
│
└── valthrun-master/             # Referans proje (Rust - CS2 cheat)
    ├── cs2/                     # CS2 SDK
    │   └── src/
    │       ├── state/          # Player state, bone data
    │       └── model.rs        # Model/bone okuma
    └── controller/              # Ana cheat controller
        └── src/enhancements/
            └── player/mod.rs    # Player ESP, aim logic
```

---

## Ana Dosyalar ve Açıklamalar

### FemboyRobin/project/utilities/offsets/offsets.hpp
**En önemli dosya!** Tüm memory offset'leri burada.
```cpp
m_pGameSceneNode = 0x330    // Entity'den scene node'a
m_modelState = 0x150        // Scene node'dan model state'e
bone_array = 0x80            // Model state'den bone matrix'e
```

### FemboyRobin/project/core/systems/impl/collector.cpp
Oyunculari toplar, bone cache'i hesaplar.
```cpp
p.bone_cache = game_scene_node + 0x150 + 0x80;
```

### FemboyRobin/project/core/systems/impl/bones.cpp
Bone pozisyonlarini okur (matrix array'den).

### FemboyRobin/project/core/features/impl/esp/player.cpp
ESP çizimi: skeleton, box, head dot, hitboxes.

### FemboyRobin/project/core/features/impl/combat/legit.cpp
Legit aimbot - mouse input simulation.

### output/ ve int/output/
Offset dump'ları. İki çeşit var:
- `output/` - Eski/güncel offsetler
- `int/output/` - İnternal cheat için güncel offsetler

---

## Offset'ler Hakkında

| Offset | Değer | Açıklama |
|--------|-------|----------|
| m_pGameSceneNode | 0x330 | Entity'den scene node'a |
| m_modelState | 0x150 | Scene node'dan model state'e |
| bone_array | 0x80 | Model state'den bone matrix'e |
| HEAD | 7 | Bone index - aimbot kullanır |

### Bone Cache Hesaplama
```cpp
game_scene_node + 0x330  // m_pGameSceneNode
         + 0x150         // m_modelState  
         + 0x80          // bone_array
= bone_cache
```

### Bone Pozisyonu Okuma
```cpp
bone_array + (bone_index * 0x20)  // her bone 0x20 (32 byte)
```

---

## Build

**Visual Studio 2022** + **x64 Release**

```bash
# FemboyRobin klasöründe
FemboyRobin.slnx aç → Build → Build Solution
```

---

## Referans Projeler

- **valthrun-master** - Rust tabanlı CS2 cheat (reference)
- **int/** - Internal cheat implementation