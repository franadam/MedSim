#include "../Headers/util.hpp"

namespace utils {
	void drawCircle(float cx, float cy, float r, int num_segments)
	{
		float theta = 3.1415926 * 2 / float(num_segments);
		float tangetial_factor = tanf(theta);//calculate the tangential factor 

		float radial_factor = cosf(theta);//calculate the radial factor 

		float x = r;//we start at angle = 0 

		float y = 0;
		glLineWidth(2);
		glBegin(GL_LINE_LOOP);
		for (int ii = 0; ii < num_segments; ii++)
		{
			glVertex2f(x + cx, y + cy);//output vertex 

			//calculate the tangential vector 
			//remember, the radial vector is (x, y) 
			//to get the tangential vector we flip those coordinates and negate one of them 

			float tx = -y;
			float ty = x;

			//add the tangential vector 

			x += tx * tangetial_factor;
			y += ty * tangetial_factor;

			//correct using the radial factor 

			x *= radial_factor;
			y *= radial_factor;
		}
		glEnd();
	}

	GLuint createTriangleVAO()
	{
		GLfloat vertices1[] = { 1,0,0, 			1,0,0, 		0.5,0,
			0,0,0, 			1,0,0, 		0.5,0.5,
			0,0,1, 		1,0,0, 		0,0,
		};

		GLfloat texCoo[] = { 0.0f, 0.0f,
			1.0f, 0.0f,
			0.5f, 1.0f };

		GLuint VBO, VAO, EBO;

		// ..:: Initialization code (done once)
		glGenVertexArrays(1, &VAO);
		// 1. Bind Vertex Array Object
		glBindVertexArray(VAO);
		// 2. Copy our vertices array in a buffer for OpenGL to use
		glGenBuffers(1, &VBO);
		glBindBuffer(GL_ARRAY_BUFFER, VBO);
		glBufferData(GL_ARRAY_BUFFER, sizeof(vertices1), vertices1, GL_STATIC_DRAW);


		// 3. Then set our vertex attributes pointers

		//position
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(GLfloat), (GLvoid*)0);
		glEnableVertexAttribArray(0);

		//color
		glEnableVertexAttribArray(1);
		glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(GLfloat), (GLvoid*)(3 * sizeof(GLfloat)));

		//texture
		glEnableVertexAttribArray(2);
		glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(GLfloat), (GLvoid*)(6 * sizeof(GLfloat)));


		//4. Unbind the VAO
		glBindVertexArray(0);

		return VAO;

	}

	GLuint createSquareVAO()
	{
		float vertices[] = {
			// first triangle
			// positions x,y,z      // colors RBG		//Texture COord
			0.5f, -0.5f, 0.0f,		1.0f, 0.0f, 0.0f,   1.0f, 0.0f,		// bottom right
			-0.5f, 0.5f, 0.0f,		0.0f, 0.0f, 1.0f,   0.0f, 1.0f,		// top left
			0.5f,  0.5f, 0.0f,		0.0f, 1.0f, 0.0f,   1.0f, 1.0f,		// top right
			// second triangle
			// positions x,y,z      // colors RBG
			0.5f, -0.5f, 0.0f,		1.0f, 0.0f, 0.0f,	1.0f, 0.0f,		// bottom right
			-0.5f, -0.5f, 0.0f,		0.0f, 1.0f, 0.0f,	0.0f, 0.0f,		// bottom left
			-0.5f,  0.5f, 0.0f,		0.0f, 0.0f, 1.0f,	0.0f, 1.0f		// top left
		};


		// ..:: Initialization code (done once (unless your object frequently changes)) :: ..
		// 1. bind Vertex Array Object
		// VAO Vertex Array Object

		unsigned int VBO, VAO, EBO;
		glGenVertexArrays(1, &VAO);
		glGenBuffers(1, &VBO);
		glGenBuffers(1, &EBO);

		// bind the Vertex Array Object first, then bind and set vertex buffer(s), and then configure vertex attributes(s).
		glBindVertexArray(VAO);

		glBindBuffer(GL_ARRAY_BUFFER, VBO);
		glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

		/*

		unsigned int indices[] = {  // note that we start from 0!
		0, 1, 3,   // first triangle
		1, 2, 3    // second triangle
		};

		// copy our index array in a element buffer for OpenGL to use
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

		*/

		// 3. then set our vertex attributes pointers
		//Linking Vertex Attributes
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0);
		glEnableVertexAttribArray(0);

		// color attribute
		glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(3 * sizeof(float)));
		glEnableVertexAttribArray(1);

		// Texture attribute
		glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(6 * sizeof(float)));
		glEnableVertexAttribArray(2);

		glBindBuffer(GL_ARRAY_BUFFER, 0);
		// remember: do NOT unbind the EBO while a VAO is active as the bound element buffer object IS stored in the VAO; keep the EBO bound.	
		//glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

		glBindVertexArray(0);

		return VAO;
	}

	GLuint createSreen2DVAO()
	{
		float vertices[] = {
			// positions            // normals         // texcoords
			25.0f, -0.5f,  25.0f,  0.0f, 1.0f, 0.0f,  25.0f,  0.0f,
			-25.0f, -0.5f,  25.0f,  0.0f, 1.0f, 0.0f,   0.0f,  0.0f,
			-25.0f, -0.5f, -25.0f,  0.0f, 1.0f, 0.0f,   0.0f, 25.0f,

			25.0f, -0.5f,  25.0f,  0.0f, 1.0f, 0.0f,  25.0f,  0.0f,
			-25.0f, -0.5f, -25.0f,  0.0f, 1.0f, 0.0f,   0.0f, 25.0f,
			25.0f, -0.5f, -25.0f,  0.0f, 1.0f, 0.0f,  25.0f, 10.0f
		};

		unsigned int VBO, VAO, EBO;
		glGenVertexArrays(1, &VAO);
		glGenBuffers(1, &VBO);
		glGenBuffers(1, &EBO);

		// bind the Vertex Array Object first, then bind and set vertex buffer(s), and then configure vertex attributes(s).
		glBindVertexArray(VAO);

		glBindBuffer(GL_ARRAY_BUFFER, VBO);
		glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

		glEnableVertexAttribArray(0);
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0);
		glEnableVertexAttribArray(1);
		glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(3 * sizeof(float)));
		glEnableVertexAttribArray(2);
		glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(6 * sizeof(float)));
		glBindVertexArray(0);

		glBindBuffer(GL_ARRAY_BUFFER, 0);


		return VAO;
	}

	GLuint createAxisVAO()
	{
		// set up vertex data (and buffer(s)) and configure vertex attributes
		// ------------------------------------------------------------------

		// Vertex input
		float vertices[] = {
			// positions x,y,z      // colors RBG
			0.0f, 0.0f, 0.0f,		1.0f, 0.0f, 0.0f,
			0.5f, 0.0f, 0.0f,		1.0f, 0.0f, 0.0f,
			0.0f, 0.0f, 0.0f,		0.0f, 0.0f, 1.0f,
			0.0f, 0.5f, 0.0f,		0.0f, 0.0f, 1.0f,
			0.0f, 0.0f, 0.0f,		0.0f, 1.0f, 0.0f,
			0.0f, 0.0f, 0.5f,		0.0f, 1.0f, 0.0f
		};

		// ..:: Initialization code (done once (unless your object frequently changes)) :: ..
		// 1. bind Vertex Array Object
		// VAO Vertex Array Object
		unsigned int VAO;
		glGenVertexArrays(1, &VAO);

		// VBO Vertex Buffer Object
		unsigned int VBO; // set memory and specify how to send the data to the graphics card
		glGenBuffers(1, &VBO); 	// 2. Copy our vertices array in a buffer for OpenGL to use

		// bind the Vertex Array Object first, then bind and set vertex buffer(s), and then configure vertex attributes(s).
		glBindVertexArray(VAO);

		glBindBuffer(GL_ARRAY_BUFFER, VBO);
		glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

		// 3. then set our vertex attributes pointers
		//Linking Vertex Attributes
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)0);
		glEnableVertexAttribArray(0);

		// color attribute
		glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)(3 * sizeof(float)));
		glEnableVertexAttribArray(1);
		// note that this is allowed, the call to glVertexAttribPointer registered VBO as the vertex attribute's bound vertex buffer object so afterwards we can safely unbind
		glBindBuffer(GL_ARRAY_BUFFER, 0);

		// You can unbind the VAO afterwards so other VAO calls won't accidentally modify this VAO, but this rarely happens. Modifying other
		// VAOs requires a call to glBindVertexArray anyways so we generally don't unbind VAOs (nor VBOs) when it's not directly necessary.
		glBindVertexArray(0);

		return VAO;
	}

	GLuint createCubeVAO()
	{
		float vertices[] = {
			-0.5f, -0.5f, -0.5f,  0.0f, 0.0f,
			0.5f, -0.5f, -0.5f,  1.0f, 0.0f,
			0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
			0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
			-0.5f,  0.5f, -0.5f,  0.0f, 1.0f,
			-0.5f, -0.5f, -0.5f,  0.0f, 0.0f,

			-0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
			0.5f, -0.5f,  0.5f,  1.0f, 0.0f,
			0.5f,  0.5f,  0.5f,  1.0f, 1.0f,
			0.5f,  0.5f,  0.5f,  1.0f, 1.0f,
			-0.5f,  0.5f,  0.5f,  0.0f, 1.0f,
			-0.5f, -0.5f,  0.5f,  0.0f, 0.0f,

			-0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
			-0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
			-0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
			-0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
			-0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
			-0.5f,  0.5f,  0.5f,  1.0f, 0.0f,

			0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
			0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
			0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
			0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
			0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
			0.5f,  0.5f,  0.5f,  1.0f, 0.0f,

			-0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
			0.5f, -0.5f, -0.5f,  1.0f, 1.0f,
			0.5f, -0.5f,  0.5f,  1.0f, 0.0f,
			0.5f, -0.5f,  0.5f,  1.0f, 0.0f,
			-0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
			-0.5f, -0.5f, -0.5f,  0.0f, 1.0f,

			-0.5f,  0.5f, -0.5f,  0.0f, 1.0f,
			0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
			0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
			0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
			-0.5f,  0.5f,  0.5f,  0.0f, 0.0f,
			-0.5f,  0.5f, -0.5f,  0.0f, 1.0f
		};

		unsigned int VBO, VAO, EBO;
		glGenVertexArrays(1, &VAO);
		glGenBuffers(1, &VBO);
		glGenBuffers(1, &EBO);

		// bind the Vertex Array Object first, then bind and set vertex buffer(s), and then configure vertex attributes(s).
		glBindVertexArray(VAO);

		glBindBuffer(GL_ARRAY_BUFFER, VBO);
		glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

		// 3. then set our vertex attributes pointers
		//Linking Vertex Attributes
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)0);
		glEnableVertexAttribArray(0);

		/*
		// color attribute
		glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)0);
		glEnableVertexAttribArray(1);
		*/

		// Texture attribute
		glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(3 * sizeof(float)));
		glEnableVertexAttribArray(2);

		glBindBuffer(GL_ARRAY_BUFFER, 0);

		glBindVertexArray(0);

		return VAO;
	}

	GLuint createLightVAO()
	{
		float vertices[] = {
			-0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,
			0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,
			0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,
			0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,
			-0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,
			-0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,

			-0.5f, -0.5f,  0.5f,  0.0f,  0.0f, 1.0f,
			0.5f, -0.5f,  0.5f,  0.0f,  0.0f, 1.0f,
			0.5f,  0.5f,  0.5f,  0.0f,  0.0f, 1.0f,
			0.5f,  0.5f,  0.5f,  0.0f,  0.0f, 1.0f,
			-0.5f,  0.5f,  0.5f,  0.0f,  0.0f, 1.0f,
			-0.5f, -0.5f,  0.5f,  0.0f,  0.0f, 1.0f,

			-0.5f,  0.5f,  0.5f, -1.0f,  0.0f,  0.0f,
			-0.5f,  0.5f, -0.5f, -1.0f,  0.0f,  0.0f,
			-0.5f, -0.5f, -0.5f, -1.0f,  0.0f,  0.0f,
			-0.5f, -0.5f, -0.5f, -1.0f,  0.0f,  0.0f,
			-0.5f, -0.5f,  0.5f, -1.0f,  0.0f,  0.0f,
			-0.5f,  0.5f,  0.5f, -1.0f,  0.0f,  0.0f,

			0.5f,  0.5f,  0.5f,  1.0f,  0.0f,  0.0f,
			0.5f,  0.5f, -0.5f,  1.0f,  0.0f,  0.0f,
			0.5f, -0.5f, -0.5f,  1.0f,  0.0f,  0.0f,
			0.5f, -0.5f, -0.5f,  1.0f,  0.0f,  0.0f,
			0.5f, -0.5f,  0.5f,  1.0f,  0.0f,  0.0f,
			0.5f,  0.5f,  0.5f,  1.0f,  0.0f,  0.0f,

			-0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,
			0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,
			0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,
			0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,
			-0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,
			-0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,

			-0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,
			0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,
			0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,
			0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,
			-0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,
			-0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f

		};


		unsigned int lightVAO, VBO;

		glGenVertexArrays(1, &lightVAO);
		glBindVertexArray(lightVAO);
		glGenBuffers(1, &VBO);

		// we only need to bind to the VBO, the container's VBO's data already contains the correct data.
		glBindBuffer(GL_ARRAY_BUFFER, VBO);
		glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

		//position
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)0);

		//normal
		glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)(3 * sizeof(float)));
		glEnableVertexAttribArray(1);

		glEnableVertexAttribArray(0);

		return lightVAO;
	}

	GLuint createVertColVAO_ext(const GLfloat* vertices, size_t sizeVertices,
		const GLfloat* colors, size_t sizeColors)
	{
		// ..:: Initialization code (done once)
		GLuint VAO, VBO;
		glGenVertexArrays(1, &VAO); std::cout << "new VAO : " << VAO << std::endl;
		// 1. Bind Vertex Array Object
		glBindVertexArray(VAO);
		// 2. Copy our vertices array in a buffer for OpenGL to use
		glGenBuffers(1, &VBO);
		glBindBuffer(GL_ARRAY_BUFFER, VBO);
		glBufferData(GL_ARRAY_BUFFER, sizeVertices, vertices, GL_STATIC_DRAW);
		// 3. Then set our vertex attributes pointers
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(GLfloat), (GLvoid*)0);
		glEnableVertexAttribArray(0);
		glGenBuffers(1, &VBO);
		glBindBuffer(GL_ARRAY_BUFFER, VBO);
		glBufferData(GL_ARRAY_BUFFER, sizeColors, colors, GL_STATIC_DRAW);
		// 3. Then set our vertex attributes pointers
		glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(GLfloat), (GLvoid*)0);
		glEnableVertexAttribArray(1);
		//4. Unbind the VAO
		glBindVertexArray(0);

		return VAO;
	}

	GLuint createWorldAxisVAO()
	{
		GLfloat vertices[] = { 0.0f, 0.0f, 0.0f,  1.0f, 0.0f, 0.0f,
								0.0f, 0.0f, 0.0f,  0.0f, 1.0f, 0.0f,
								0.0f, 0.0f, 0.0f,  0.0f, 0.0f, 1.0f };
		GLfloat colors[] = { 1.0f, 0.0f, 0.0f,  1.0f, 0.0f, 0.0f,
								0.0f, 1.0f, 0.0f,  0.0f, 1.0f, 0.0f,
								0.0f, 0.0f, 1.0f,  0.0f, 0.0f, 1.0f };
		return createVertColVAO(vertices, colors);
	}

	GLuint createPosColTexVAOEBO_ext(const GLfloat* vertices, size_t sizeVertices,
		const GLuint* indices, size_t sizeIndices)
	{
		unsigned int VBO, VAO, EBO;
		glGenVertexArrays(1, &VAO);
		glGenBuffers(1, &VBO);
		glGenBuffers(1, &EBO);

		// bind the Vertex Array Object first, then bind and set vertex buffer(s), and then configure vertex attributes(s).
		glBindVertexArray(VAO);

		glBindBuffer(GL_ARRAY_BUFFER, VBO);
		glBufferData(GL_ARRAY_BUFFER, sizeVertices, vertices, GL_STATIC_DRAW);

		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeIndices, indices, GL_STATIC_DRAW);

		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(0 * sizeof(float)));
		glEnableVertexAttribArray(0);

		glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(3 * sizeof(float)));
		glEnableVertexAttribArray(1);

		glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(6 * sizeof(float)));
		glEnableVertexAttribArray(2);

		// note that this is allowed, the call to glVertexAttribPointer registered VBO as the vertex attribute's bound vertex buffer object so afterwards we can safely unbind
		glBindBuffer(GL_ARRAY_BUFFER, 0);

		// remember: do NOT unbind the EBO while a VAO is active as the bound element buffer object IS stored in the VAO; keep the EBO bound.
		//glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

		// You can unbind the VAO afterwards so other VAO calls won't accidentally modify this VAO, but this rarely happens. Modifying other
		// VAOs requires a call to glBindVertexArray anyways so we generally don't unbind VAOs (nor VBOs) when it's not directly necessary.
		glBindVertexArray(0);

		return VAO;
	}

	GLuint createQuadVAO()
	{
		float tile = 1.0f;
		GLfloat vertices[] = {
			// positions          // colors           // texture coords
			 0.5f,  0.5f, 0.0f,   1.0f, 0.0f, 0.0f,   tile, tile,   // top right
			 0.5f, -0.5f, 0.0f,   0.0f, 1.0f, 0.0f,   tile, 0.0f,   // bottom right
			-0.5f, -0.5f, 0.0f,   0.0f, 0.0f, 1.0f,   0.0f, 0.0f,   // bottom left
			-0.5f,  0.5f, 0.0f,   1.0f, 1.0f, 0.0f,   0.0f, tile    // top left 
		};

		GLuint indices[] = {
				0, 3, 1,
				1, 3, 2
		};

		return createPosColTexVAOEBO(vertices, indices);
	}

	GLuint createPosTargetVAO()
	{
		GLfloat vertices[] = { -0.1f,  0.0f,  0.0f,   0.1f,  0.0f,  0.0f,
								0.0f, -0.1f,  0.0f,   0.0f,  0.1f,  0.0f,
								0.0f,  0.0f, -0.1f,   0.0f,  0.0f,  0.1f };
		GLfloat colors[] = { 0.6f, 0.6f, 1.0f,  0.6f, 0.6f, 1.0f,
								0.6f, 0.6f, 1.0f,  0.6f, 0.6f, 1.0f,
								0.6f, 0.6f, 1.0f,  0.6f, 0.6f, 1.0f };
		return createVertColVAO(vertices, colors);
	}


	GLuint createCubeVAO(glm::vec3 mini, glm::vec3 maxi)
	{
		GLfloat vertices[] = { mini.x, mini.y, mini.z,  mini.x, maxi.y, mini.z,
								mini.x, mini.y, maxi.z,  mini.x, maxi.y, maxi.z,
								mini.x, mini.y, mini.z,  mini.x, mini.y, maxi.z,
								mini.x, maxi.y, mini.z,  mini.x, maxi.y, maxi.z,

								maxi.x, mini.y, mini.z,  maxi.x, maxi.y, mini.z,
								maxi.x, mini.y, maxi.z,  maxi.x, maxi.y, maxi.z,
								maxi.x, mini.y, mini.z,  maxi.x, mini.y, maxi.z,
								maxi.x, maxi.y, mini.z,  maxi.x, maxi.y, maxi.z,

								mini.x, mini.y, mini.z,  maxi.x, mini.y, mini.z,
								mini.x, mini.y, maxi.z,  maxi.x, mini.y, maxi.z,
								mini.x, maxi.y, mini.z,  maxi.x, maxi.y, mini.z,
								mini.x, maxi.y, maxi.z,  maxi.x, maxi.y, maxi.z,
		};

		GLfloat colors[3 * 24] = { 0.0f };

		return createVertColVAO(vertices, colors);
	}

	GLuint createBoundingBoxVAO(aiVector3D mini, aiVector3D maxi)
	{
		GLfloat vertices[] = { mini.x, mini.y, mini.z,  mini.x, maxi.y, mini.z,
								mini.x, mini.y, maxi.z,  mini.x, maxi.y, maxi.z,
								mini.x, mini.y, mini.z,  mini.x, mini.y, maxi.z,
								mini.x, maxi.y, mini.z,  mini.x, maxi.y, maxi.z,

								maxi.x, mini.y, mini.z,  maxi.x, maxi.y, mini.z,
								maxi.x, mini.y, maxi.z,  maxi.x, maxi.y, maxi.z,
								maxi.x, mini.y, mini.z,  maxi.x, mini.y, maxi.z,
								maxi.x, maxi.y, mini.z,  maxi.x, maxi.y, maxi.z,

								mini.x, mini.y, mini.z,  maxi.x, mini.y, mini.z,
								mini.x, mini.y, maxi.z,  maxi.x, mini.y, maxi.z,
								mini.x, maxi.y, mini.z,  maxi.x, maxi.y, mini.z,
								mini.x, maxi.y, maxi.z,  maxi.x, maxi.y, maxi.z,
		};

		GLfloat colors[3 * 24] = { 0.0f };

		return createVertColVAO(vertices, colors);
	}

	void draw_cylinder(GLfloat radius,
		GLfloat height,
		GLubyte R,
		GLubyte G,
		GLubyte B)
	{
		GLfloat x = 0.0;
		GLfloat y = 0.0;
		GLfloat angle = 0.0;
		GLfloat angle_stepsize = 0.1;

		/** Draw the tube */
		glColor3ub(R - 40, G - 40, B - 40);
		glBegin(GL_QUAD_STRIP);
		angle = 0.0;
		while (angle < 2 * 3.141) {
			x = radius * cos(angle);
			y = radius * sin(angle);
			glVertex3f(x, y, height);
			glVertex3f(x, y, 0.0);
			angle = angle + angle_stepsize;
		}
		glVertex3f(radius, 0.0, height);
		glVertex3f(radius, 0.0, 0.0);
		glEnd();

		/** Draw the circle on top of cylinder */
		glColor3ub(R, G, B);
		glBegin(GL_POLYGON);
		angle = 0.0;
		while (angle < 2 * 3.141) {
			x = radius * cos(angle);
			y = radius * sin(angle);
			glVertex3f(x, y, height);
			angle = angle + angle_stepsize;
		}
		glVertex3f(radius, 0.0, height);
		glEnd();
	}

	void display() {
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		glMatrixMode(GL_MODELVIEW);
	}

}