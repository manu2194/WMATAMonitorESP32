"""
PlatformIO build script to load environment variables from .env file.

This script runs before the build and reads the .env file to inject
environment variables as compiler definitions (-D flags).

Usage:
    1. Create a .env file in the project root with:
       WMATA_API_KEY=your_api_key_here
    
    2. In your C++ code, use the macro:
       const char* apiKey = WMATA_API_KEY;
"""

import os

def load_env_file(env_path):
    """
    Parse a .env file and return a dictionary of key-value pairs.
    
    :param str env_path: Path to the .env file
    :return dict: Dictionary of environment variables
    """
    env_vars = {}
    
    if not os.path.exists(env_path):
        print(f"Warning: .env file not found at {env_path}")
        return env_vars
    
    with open(env_path, 'r') as f:
        for line in f:
            line = line.strip()
            # Skip empty lines and comments
            if not line or line.startswith('#'):
                continue
            # Parse KEY=VALUE
            if '=' in line:
                key, value = line.split('=', 1)
                env_vars[key.strip()] = value.strip()
    
    return env_vars


def before_build(source, target, env):
    """
    PlatformIO pre-build hook to inject environment variables.
    
    :param source: Build source files
    :param target: Build target
    :param env: PlatformIO environment
    """
    pass  # Called before build, but we use Import("env") directly


# Main script execution
Import("env")

# Path to .env file (in project root)
project_dir = env.get("PROJECT_DIR", ".")
env_path = os.path.join(project_dir, ".env")

# Load environment variables
env_vars = load_env_file(env_path)

# Add each variable as a compiler definition
# The value is wrapped in escaped quotes so it becomes a string literal in C++
build_flags = []

if "WMATA_API_KEY" in env_vars:
    api_key = env_vars["WMATA_API_KEY"]
    # Escape quotes for the compiler: -DWMATA_API_KEY=\"value\"
    build_flags.append(f'-DWMATA_API_KEY=\\"{api_key}\\"')
    print(f"✓ WMATA_API_KEY loaded from .env")
else:
    print("⚠ Warning: WMATA_API_KEY not found in .env file")
    print("  Create a .env file with: WMATA_API_KEY=your_key_here")

if "STATION_CODE" in env_vars:
    station_code = env_vars["STATION_CODE"]
    build_flags.append(f'-DSTATION_CODE=\\"{station_code}\\"')
    print(f"✓ STATION_CODE loaded from .env: {station_code}")
else:
    # Default to B35 (NoMA-Gallaudet U) if not specified
    build_flags.append('-DSTATION_CODE=\\"B35\\"')
    print("ℹ STATION_CODE not in .env, using default: B35")

# Append build flags to the environment
env.Append(CPPDEFINES=[])
for flag in build_flags:
    env.Append(BUILD_FLAGS=[flag])
