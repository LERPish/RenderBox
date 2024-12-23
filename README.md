# **RenderBox**

RenderBox is a simple software-based 3D renderer built from scratch in C. It supports basic 3D transformations, wireframe rendering, and is designed to be extensible with future features like rasterization, .obj file importing, and a custom shader language.

## **Features**
- 3D Object transformations (translation, rotation, scaling).
- Wireframe rendering using a software pipeline.

## **Planned Features**
- **File Importing:** Parse and load .obj files for more complex models.
- **Rasterization:** Implement a software rasterizer for 3D polygon rendering.
- **Camera Enhancements:** Add full 6-DOF (Degrees of Freedom) camera movement.
- **Custom Shader Language:** Develop a basic language for programmable rendering.

## **How to Build**
1. Clone the repository:
   ```bash
   git clone <repository-url>

2. Build the project using make:
   ```bash
   cd RenderBox
   make

3. Run the executable:
   ```bash
   ./build/renderer
