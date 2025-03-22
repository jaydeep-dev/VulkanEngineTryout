%VULKAN_SDK%\Bin\glslc.exe ./Shaders/simple_shader.vert -o ./Shaders/simple_shader.vert.spv
%VULKAN_SDK%\Bin\glslc.exe ./Shaders/simple_shader.frag -o ./Shaders/simple_shader.frag.spv
echo "Compiled shaders successfully"
exit 0