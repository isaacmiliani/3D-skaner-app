#version 120

    attribute vec4 in_vertex;

    uniform mat4 model;
    uniform mat4 view;
    uniform mat4 projection;

    void main()
    {
        gl_Position = projection * view * model * in_vertex;
    }
