# my_engine_cpp

Petit moteur OpenGL d'exemple (GLFW + GLEW + glm) — instructions d'installation, compilation et exécution.

Ce README explique comment préparer un environnement de développement, compiler et lancer le projet sur Linux et macOS.

## Prérequis (packages)

Les dépendances principales sont :
- GNU C++ (g++)
- make
- pkg-config
- GLFW (dev)
- GLEW (dev)
- GLM (headers)
- FreeGLUT (optionnel selon le code)

Exemples d'installation par distribution :

- Debian / Ubuntu :

```bash
sudo apt update
sudo apt install build-essential pkg-config git cmake \
    libglfw3-dev libglew-dev libglm-dev freeglut3-dev
```

- Fedora :

```bash
sudo dnf install @development-tools pkgconfig git \
    glfw-devel glew-devel glm-devel freeglut-devel
```

- Arch Linux / Manjaro :

```bash
sudo pacman -Syu base-devel pkgconf git \
    glfw-x11 glew glm freeglut
```

- macOS (Homebrew) :

```bash
brew install pkg-config glfw glew glm freeglut
```

Remarque : les noms de paquets peuvent varier selon la version et la distribution. Le fichier `makefile` utilise `pkg-config --cflags glfw3` pour détecter GLFW (amené à mise à jour).

## Compiler

1. Dans le terminal ouvert dans le repertoire du projet :

```bash
cd /home/${USER}/Desktop/my_engine_cpp
```

2. Compilation :

```bash
make
```

## Contrôles

Le projet utilise un contrôleur de type FPS simple (via la classe `Camera`). Par défaut :

- W / A / S / D : avancer / gauche / reculer / droite
- Espace : monter
- Left Ctrl : descendre
- Left Shift : courir / accélérer
- Clic gauche + mouvement souris : regarder autour (le premier clic recentre le curseur)