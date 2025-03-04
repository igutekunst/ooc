# ooc

ooc is a C library created with the Clyde package manager.

## Features

- Simple calculator functionality
- Version information

## API

```c
// Add two numbers
int ooc_add(int a, int b);

// Get library version
const char* ooc_version(void);
```

## Building

```bash
clyde build
```

## Project Structure

```
ooc/
├── src/                  # Implementation files
│   └── ooc.c
├── include/              # Public headers
│   └── ooc/
│       └── ooc.h
└── private_include/      # Private headers (if any)
```

## Configuration

See `package.yml` for build settings and dependencies.

## Usage

To use this library in another Clyde project, add it to the dependencies in `package.yml`:

```yaml
requires:
  ooc: 
    path: ../ooc
```

## Include Path Structure

Headers are organized to prevent naming conflicts:

```
ooc/
├── include/              # Public headers (exposed to dependents)
│   └── ooc/      # Package namespace directory
│       └── ooc.h
└── private_include/     # Private headers (not exposed)
```

Public headers are included using the package name as namespace:
```c
#include <ooc/ooc.h>
```

This structure ensures:
- No header name conflicts between packages
- Clear separation of public/private interfaces
- No need to copy headers between packages 