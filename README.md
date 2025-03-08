# UMS

Developed with Unreal Engine 5

ProjectName/
├── Content/                     # All game assets go here
│   ├── Art/                     # Visual assets (textures, models, materials, etc.)
│   │   ├── Characters/          # Character models, skeletons, animations
│   │   ├── Environments/        # Environment assets (meshes, props, etc.)
│   │   ├── Materials/           # Material files and instances
│   │   ├── Textures/            # Texture files
│   │   └── VFX/                 # Visual effects (particles, Niagara systems)
│   ├── Audio/                   # Sound effects and music
│   ├── Blueprints/              # Blueprint classes
│   │   ├── Characters/          # Character-specific blueprints
│   │   ├── GameModes/           # Game mode blueprints
│   │   ├── UI/                  # User interface blueprints
│   │   └── Utilities/           # Utility blueprints (e.g., managers, helpers)
│   ├── Maps/                    # Level files
│   │   ├── MainMenu/            # Main menu level
│   │   ├── Levels/              # Gameplay levels
│   │   └── Tests/               # Test levels
│   ├── UI/                      # User interface assets (UMG widgets, fonts, etc.)
│   ├── Data/                    # Data assets (e.g., data tables, curves)
│   ├── Plugins/                 # Custom or third-party plugins
│   └── ThirdParty/              # Third-party assets (e.g., marketplace content)
├── Config/                      # Configuration files (e.g., INI files)
├── Source/                      # C++ source code (if using C++)
│   ├── ProjectName/             # Game-specific C++ classes
│   │   ├── Characters/          # Character classes
│   │   ├── GameModes/           # Game mode classes
│   │   ├── UI/                  # UI-related classes
│   │   └── Utilities/           # Utility classes
│   └── ThirdParty/              # Third-party C++ libraries
├── Saved/                       # Autosaves, logs, and other auto-generated files
├── Intermediate/                # Intermediate files (e.g., shaders, build files)
└── External/                    # External dependencies (e.g., documentation, design files)