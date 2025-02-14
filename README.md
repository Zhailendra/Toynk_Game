# Toynk

🔥 **Toynk** is a fast-paced tank battle game inspired by Wii Tank, built in **Unreal Engine** using **C++** and **Blueprints** for the UI. Outsmart enemies, upgrade your tank, find hidden coins 💰, and dominate the battlefield! 💣🚜⚔️🚀🎮

## 🎮 Features
- **Intense Tank Combat** – Battle against a variety of enemy tanks using bullets and mines.
- **Upgradeable Tanks** – Improve your tank's abilities after each level.
- **Hidden Coins** – Discover secret coins in barrels to gain extra currency.
- **10 Levels** – More can be easily added for extended gameplay.
- **Modular Enemy System** – Easily create new enemy types with unique behaviors.
- **Ultra-Modular Object Pooling System** – Used for **all projectiles and mines**, ensuring efficient memory usage and performance.

## 🛠️ Development Details
- Built with **Unreal Engine**.
- Core gameplay coded in **C++**.
- UI designed using **Blueprints**.
- **Modular Codebase** – All tanks inherit from `BaseTank`, allowing easy creation of new tank types with different abilities.

### 📂 Important Files
- **BaseTank Class:** [BaseTank](Source/Toynk/Public/Tank/BaseTank.h)
- **Object Pooling System:** [PoolSubsystem](Source/Toynk/Public/ObjectPoolIng/PoolSubsystem.h), [Poolable](Source/Toynk/Public/ObjectPoolIng/Poolable.h)
- **Projectile & Mine Pooling Usage:** [Bullet](Source/Toynk/Public/Tank/Bullet/Bullet.h), [LandMine](Source/Toynk/Public/Tank/LandMine/LandMine.h)

## 🚀 Adding More Content
Toynk is designed for easy expansion:
- **Adding New Levels:** Simply extend the level list and configure enemy spawns.
- **Creating New Enemies:** Inherit from `BaseTank`, define new behaviors, and integrate them into the game world.
- **Using Object Pooling:** Spawn new projectiles or mines efficiently using the provided system.

## 👥 Contributors
- [@Zhailendra](https://github.com/Zhailendra)
- [@BySkullZ](https://github.com/BySkullZ)

---
Feel free to fork and contribute to this project! 🚀🔥

