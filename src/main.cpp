/* Lab 5 base code - transforms using local matrix functions
	to be written by students -
	CPE 471 Cal Poly Z. Wood + S. Sueda
*/
#include <iostream>
#include <glad/glad.h>

#include "GLSL.h"
#include "Program.h"
#include "Shape.h"
#include "WindowManager.h"

// used for helper in perspective
#include "glm/glm.hpp"

using namespace std;
using namespace glm;

class Matrix
{

public:

	static void printMat(float *A, const char *name = 0)
	{
		// OpenGL uses col-major ordering:
		// [ 0  4  8 12]
		// [ 1  5  9 13]
		// [ 2  6 10 14]
		// [ 3  7 11 15]
		// The (i, j)th element is A[i+4*j].

		if (name)
		{
			printf("%s=[\n", name);
		}

		for (int i = 0; i < 4; ++i)
		{
			for (int j = 0; j < 4; ++j)
			{
				printf("%- 5.2f ", A[i + 4*j]);
			}
			printf("\n");
		}

		if (name)
		{
			printf("];");
		}
		printf("\n");
	}

	static void createIdentityMat(float *M)
	{
		// set all values to zero
		for (int i = 0; i < 4; ++i)
		{
			for (int j = 0; j < 4; ++j)
			{
				M[i + 4*j] = 0;
			}
		}

		// overwrite diagonal with 1s
		M[0] = M[5] = M[10] = M[15] = 1;
	}

	static void createTranslateMat(float *T, float x, float y, float z)
	{
		// IMPLEMENT ME
                
		T[ 0] = 1.0f;
		T[ 1] = 0.0f;
		T[ 2] = 0.0f;
		T[ 3] = 0.0f;

		T[ 4] = 0.0f;
                T[ 5] = 1.0f;
		T[ 6] = 0.0f;
		T[ 7] = 0.0f;

		T[ 8] = 0.0f;
                T[ 9] = 0.0f;
		T[10] = 1.0f;
		T[11] = 0.0f;

		T[12] = x;
                T[13] = y;
		T[14] = z;
		T[15] = 1.0f;
	}
	

	static void createScaleMat(float *S, float x, float y, float z)
	{
		// IMPLEMENT ME
		S[ 0] = x;
		S[ 1] = 0.0f;
		S[ 2] = 0.0f;
		S[ 3] = 0.0f;

		S[ 4] = 0.0f;
                S[ 5] = y;
		S[ 6] = 0.0f;
		S[ 7] = 0.0f;

		S[ 8] = 0.0f;
                S[ 9] = 0.0f;
		S[10] = z;
		S[11] = 0.0f;

		S[12] = 0.0f;
                S[13] = 0.0f;
		S[14] = 0.0f;
		S[15] = 1.0f;
	}

	static void createRotateMatX(float *R, float radians)
	{
		// IMPLEMENT ME
		R[ 0] = 1.0f;
		R[ 1] = 0.0f;
		R[ 2] = 0.0f;
		R[ 3] = 0.0f;

		R[ 4] = 0.0f;
                R[ 5] = cos(radians);
		R[ 6] = sin(radians);
		R[ 7] = 0.0f;

		R[ 8] = 0.0f;
                R[ 9] = -sin(radians);
		R[10] = cos(radians);
		R[11] = 0.0f;

		R[12] = 0.0f;
                R[13] = 0.0f;
		R[14] = 0.0f;
		R[15] = 1.0f;
	}

	static void createRotateMatY(float *R, float radians)
	{
		// IMPLEMENT ME
		R[ 0] = cos(radians);
		R[ 1] = 0.0f;
		R[ 2] = -sin(radians);
		R[ 3] = 0.0f;

		R[ 4] = 0.0f;
                R[ 5] = 1.0f;
		R[ 6] = 0.0f;
		R[ 7] = 0.0f;

		R[ 8] = sin(radians);
                R[ 9] = 0.0f;
		R[10] = cos(radians);
		R[11] = 0.0f;

		R[12] = 0.0f;
                R[13] = 0.0f;
		R[14] = 0.0f;
		R[15] = 1.0f;
	}

	static void createRotateMatZ(float *R, float radians)
	{
		// IMPLEMENT ME
		R[ 0] = cos(radians);
		R[ 1] = sin(radians);
		R[ 2] = 0.0f;
		R[ 3] = 0.0f;

		R[ 4] = -sin(radians);
                R[ 5] = cos(radians);
		R[ 6] = 0.0f;
		R[ 7] = 0.0f;

		R[ 8] = 0.0f;
                R[ 9] = 0.0f;
		R[10] = 1.0f;
		R[11] = 0.0f;

		R[12] = 0.0f;
                R[13] = 0.0f;
		R[14] = 0.0f;
		R[15] = 1.0f;
	}

	static void multMat(float *C, const float *A, const float *B)
	{
		float c = 0;

		for (int k = 0; k < 4; ++k)
		{
			// Process kth column of C
			for (int i = 0; i < 4; ++i)
			{
				// Process ith row of C.
				// The (i,k)th element of C is the dot product
				// of the ith row of A and kth col of B.
				c = 0.0;

				// vector dot product
				for (int j = 0; j < 4; ++j)
				{
					// IMPLEMENT ME
                                        c += A[i+(4*j)]*B[j+(4*k)];
				}
                                C[i+(4*k)] = c;

			}
		}
	}

	static void createPerspectiveMat(float *m, float fovy, float aspect, float zNear, float zFar)
	{
		// http://www-01.ibm.com/support/knowledgecenter/ssw_aix_61/com.ibm.aix.opengl/doc/openglrf/gluPerspective.htm%23b5c8872587rree
		float f = 1.0f / glm::tan(0.5f * fovy);

		m[ 0] = f / aspect;
		m[ 1] = 0.0f;
		m[ 2] = 0.0f;
		m[ 3] = 0.0f;
		m[ 4] = 0;

		m[ 5] = f;
		m[ 6] = 0.0f;
		m[ 7] = 0.0f;
		m[ 8] = 0.0f;

		m[ 9] = 0.0f;
		m[10] = (zFar + zNear) / (zNear - zFar);
		m[11] = -1.0f;
		m[12] = 0.0f;

		m[13] = 0.0f;
		m[14] = 2.0f * zFar * zNear / (zNear - zFar);
		m[15] = 0.0f;
	}

};

class Application : public EventCallbacks
{

public:

	WindowManager * windowManager = nullptr;
        float angle = 0;
	// Our shader program
	std::shared_ptr<Program> prog;
	std::shared_ptr<Program> prog2;
	std::shared_ptr<Program> prog3;
	std::shared_ptr<Program> prog4;

	// Shape to be used (from obj file)
	shared_ptr<Shape> shape;

	// Contains vertex information for OpenGL
	GLuint VertexArrayID;
        GLuint Cylinder_vertex;
        GLuint Cylinder_buffer;
	// Data necessary to give our triangle to OpenGL
	GLuint VertexBufferID;
        GLuint colorbuffer;
	void keyCallback(GLFWwindow *window, int key, int scancode, int action, int mods)
	{
		if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
		{
			glfwSetWindowShouldClose(window, GL_TRUE);
		}
                if (key ==GLFW_KEY_A && action ==GLFW_PRESS){
                        angle-=2*M_PI/30.0;
                }
                if (key ==GLFW_KEY_D && action ==GLFW_PRESS){
                        angle+=2*M_PI/30.0;
                }
	}

	void mouseCallback(GLFWwindow *window, int button, int action, int mods)
	{
		double posX, posY;

		if (action == GLFW_PRESS)
		{
			glfwGetCursorPos(window, &posX, &posY);
			cout << "Pos X " << posX <<  " Pos Y " << posY << endl;
		}
	}

	void resizeCallback(GLFWwindow *window, int width, int height)
	{
		glViewport(0, 0, width, height);
	}

	void init(const std::string& resourceDirectory)
	{
		GLSL::checkVersion();

		// Set background color.
		glClearColor(0.12f, 0.34f, 0.56f, 1.0f);

		// Enable z-buffer test.
		glEnable(GL_DEPTH_TEST);
                glEnable (GL_BLEND); 
                glBlendFunc (GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
		// Initialize the GLSL program.
		prog = make_shared<Program>();
		prog->setVerbose(true);
		prog->setShaderNames(resourceDirectory + "/simple_vert.glsl", resourceDirectory + "/simple_frag.glsl");
		if (! prog->init())
		{
			std::cerr << "One or more shaders failed to compile... exiting!" << std::endl;
			exit(1);
		}
		prog->init();
		prog->addUniform("P");
		prog->addUniform("MV");
                prog->addUniform("Color");
		prog->addAttribute("vertPos");
		prog->addAttribute("vertNor");
                //ball
		prog2 = make_shared<Program>();
		prog2->setVerbose(true);
		prog2->setShaderNames(resourceDirectory + "/ball_vert.glsl", resourceDirectory + "/ball_frag.glsl");
		if (! prog2->init())
		{
			std::cerr << "One or more shaders failed to compile... exiting!" << std::endl;
			exit(1);
		}
		prog2->init();
		prog2->addUniform("P");
		prog2->addUniform("MV");
		prog2->addAttribute("vertPos");
		prog2->addAttribute("vertNor");
                //cylinder
		prog3 = make_shared<Program>();
		prog3->setVerbose(true);
		prog3->setShaderNames(resourceDirectory + "/cylinder_vert.glsl", resourceDirectory + "/cylinder_frag.glsl");
		if (! prog3->init())
		{
			std::cerr << "One or more shaders failed to compile... exiting!" << std::endl;
			exit(1);
		}
		prog3->init();
		prog3->addUniform("P");
		prog3->addUniform("MV");
                prog3->addAttribute("cylinder_vertex");
                prog3->addAttribute("cylinder_color");
                //snow
		prog4 = make_shared<Program>();
		prog4->setVerbose(true);
		prog4->setShaderNames(resourceDirectory + "/snow_vert.glsl", resourceDirectory + "/snow_frag.glsl");
		if (! prog4->init())
		{
			std::cerr << "One or more shaders failed to compile... exiting!" << std::endl;
			exit(1);
		}
		prog4->init();
		prog4->addUniform("P");
		prog4->addUniform("MV");
		prog4->addAttribute("vertPos");
		prog4->addAttribute("vertNor");
                
	}

	void initGeom(const std::string& resourceDirectory)
	{
		// Initialize mesh.
		shape = make_shared<Shape>();
		shape->loadMesh(resourceDirectory + "/sphere.obj");
		shape->resize();
		shape->init();
	        //The sun
		//generate the VAO
		glGenVertexArrays(1, &Cylinder_vertex);
		glBindVertexArray(Cylinder_vertex);

		//generate vertex buffer to hand off to OGL
		glGenBuffers(1, &Cylinder_buffer);
		//set the current state to focus on our vertex buffer
		glBindBuffer(GL_ARRAY_BUFFER, Cylinder_buffer);

                //generate points around circle
                float delta = 2*M_PI/80.0;
		GLfloat g_vertex_buffer_data[81*3];
                g_vertex_buffer_data[0] = 0.0;
                g_vertex_buffer_data[1] = 0.0;
                
                for (int i = 0; i<81; i++){
                       g_vertex_buffer_data[(i+1)*3] = 0.7*cos(delta*i);
                       g_vertex_buffer_data[(i+1)*3+1] = 0.7*sin(delta*i);
                       g_vertex_buffer_data[(i+1)*3+2] = i%2;
                }
	 
		//we need to set up the vertex array
		glEnableVertexAttribArray(3);
		//key function to get up how many elements to pull out at a time (3)
		glVertexAttribPointer(3, 3, GL_FLOAT, GL_FALSE, 0, (void*) 0);

		//actually memcopy the data - only do this once
		glBufferData(GL_ARRAY_BUFFER, sizeof(g_vertex_buffer_data), g_vertex_buffer_data, GL_DYNAMIC_DRAW);
		

                unsigned int indicies[80*3];
                for (int i = 0; i<=77;i++){
                     indicies[i*3] = i+1;
                     indicies[i*3+1] = i+2;
                     indicies[i*3+2] = i+3;
                }
                indicies[78*3] = 79;
                indicies[78*3+1] = 80;
                indicies[78*3+2] = 1;
                indicies[79*3] = 80;
                indicies[79*3+1] = 1;
                indicies[79*3+2] = 2;
                unsigned int EBO;
                glGenBuffers(1, &EBO);
                glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
                glBufferData(GL_ELEMENT_ARRAY_BUFFER,sizeof(indicies), indicies, GL_STATIC_DRAW);
                
		glGenBuffers(1,&colorbuffer);
		glBindBuffer(GL_ARRAY_BUFFER, colorbuffer);
                static GLfloat g_color_buffer_data[81*3];
		g_color_buffer_data[0] = 1.0;
                g_color_buffer_data[1] = 0.4;
                g_color_buffer_data[2] = 0.5;
                for (int i = 1; i<=80;i++){
                  g_color_buffer_data[i*3] = 0.6;
                  g_color_buffer_data[i*3+1] = 0.2;
                  g_color_buffer_data[i*3+2] = 0.3;
                }

                glBufferData(GL_ARRAY_BUFFER, sizeof(g_color_buffer_data), g_color_buffer_data, GL_DYNAMIC_DRAW);
                glEnableVertexAttribArray(4);
		glVertexAttribPointer(4,3,GL_FLOAT,GL_FALSE,0, (void*)0);
		glBindVertexArray(0);
	}


	void render()
	{
		// Local modelview matrix use this for lab 5
		float MV[16] = {0};
		float Temp[16] = {0};
		float Temp2[16] = {0};
		float P[16] = {0};
		float S[16] = {0};
		float R[16] = {0};
		float T[16] = {0};
                float Color[3] = {1.0,1.0,1.0}; 
                float Tg[16] = {0};
                float Rg[16] = {0};
                float Rz[16] = {0};
                float Time = glfwGetTime();
		// Get current frame buffer size.
		int width, height;
		glfwGetFramebufferSize(windowManager->getHandle(), &width, &height);
		glViewport(0, 0, width, height);

		// Clear framebuffer.
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		// Use the local matrices for lab 5
		float aspect = width/(float)height;
		Matrix::createPerspectiveMat(P, 70.0f, aspect, 0.1f, 100.0f);
                Matrix::createRotateMatY(Rg,angle);//global ratation

                Matrix::createTranslateMat(Tg, 0.0f, 0.0f, -10.0f); //Global Translate
                  //bottom
                Color[0] = 1.0;
                Color[1] =1.0; //black
                Color[2] = 1.0;
                Matrix::createScaleMat(S,2.0f,1.4f,1.8f);           //scale
                Matrix::createTranslateMat(T, 0.0f, -1.7f, 0.0f);    //local Translate
                Matrix::multMat(Temp2,T, S);
                Matrix::multMat(Temp,Rg,Temp2);
                Matrix::multMat(MV, Tg, Temp);
		prog->bind();
                glUniform3fv(prog->getUniform("Color"),1,Color);
		glUniformMatrix4fv(prog->getUniform("P"), 1, GL_FALSE, P);
		glUniformMatrix4fv(prog->getUniform("MV"), 1, GL_FALSE, MV);
		shape->draw(prog);
		prog->unbind();
                  //middle
                Matrix::createScaleMat(S,1.2f,0.9f,1.1f);           //scale
                Matrix::createTranslateMat(T, 0.0f, 0.0f, 0.0f);    //local Translate
                Matrix::multMat(Temp2,T, S);
                Matrix::multMat(Temp,Rg,Temp2);
                Matrix::multMat(MV, Tg, Temp);
		prog->bind();
                glUniform3fv(prog->getUniform("Color"),1,Color);
		glUniformMatrix4fv(prog->getUniform("P"), 1, GL_FALSE, P);
		glUniformMatrix4fv(prog->getUniform("MV"), 1, GL_FALSE, MV);
		shape->draw(prog);
		prog->unbind();
                //top
                Matrix::createScaleMat(S,1.3f,1.0f,1.2f);           //scale
                Matrix::createTranslateMat(T, 0.0f, 1.3f, 0.0f);    //local Translate
                Matrix::multMat(Temp2,T, S);
                Matrix::multMat(Temp,Rg,Temp2);
                Matrix::multMat(MV, Tg, Temp);
		prog->bind();
                glUniform3fv(prog->getUniform("Color"),1,Color);
		glUniformMatrix4fv(prog->getUniform("P"), 1, GL_FALSE, P);
		glUniformMatrix4fv(prog->getUniform("MV"), 1, GL_FALSE, MV);
		shape->draw(prog);
		prog->unbind();
                //right_eye
                Matrix::createScaleMat(S,0.1f,0.1f,0.1f);           //scale
                Matrix::createTranslateMat(T, 0.5f, 1.2f, 1.0f);    //local Translate
                Matrix::multMat(Temp2,T, S);
                Matrix::multMat(Temp,Rg,Temp2);
                Matrix::multMat(MV, Tg, Temp);

                Color[0] = 0.0;
                Color[1] =0.0; //black
                Color[2] = 0.0;
		prog->bind();
                glUniform3fv(prog->getUniform("Color"),1,Color);
		glUniformMatrix4fv(prog->getUniform("P"), 1, GL_FALSE, P);
		glUniformMatrix4fv(prog->getUniform("MV"), 1, GL_FALSE, MV);
		shape->draw(prog);
		prog->unbind();
                //left_eye
                Matrix::createScaleMat(S,0.1f,0.1f,0.1f);           //scale
                Matrix::createTranslateMat(T, -0.5f, 1.2f, 1.0f);    //local Translate
                Matrix::multMat(Temp2,T, S);
                Matrix::multMat(Temp,Rg,Temp2);
                Matrix::multMat(MV, Tg, Temp);
		prog->bind();
                glUniform3fv(prog->getUniform("Color"),1,Color);
		glUniformMatrix4fv(prog->getUniform("P"), 1, GL_FALSE, P);
		glUniformMatrix4fv(prog->getUniform("MV"), 1, GL_FALSE, MV);
		shape->draw(prog);
		prog->unbind();
                //left_arm
                Color[0] = 0.9;
                Color[1] =0.6; //black
                Color[2] = 0.3;
                Matrix::createScaleMat(S,0.8f,0.08f,0.08f);           //scale
                Matrix::createRotateMatZ(R,-0.7);
                Matrix::createTranslateMat(T, -1.5f, 0.2f, 0.2f);    //local Translate
                Matrix::multMat(Temp,R,S);
                Matrix::multMat(Temp2,T, Temp);
                Matrix::multMat(Temp,Rg,Temp2);
                Matrix::multMat(MV, Tg, Temp);
		prog->bind();
                glUniform3fv(prog->getUniform("Color"),1,Color);
		glUniformMatrix4fv(prog->getUniform("P"), 1, GL_FALSE, P);
		glUniformMatrix4fv(prog->getUniform("MV"), 1, GL_FALSE, MV);
		shape->draw(prog);
		prog->unbind();
                //right_arm
                Color[0] = 0.9;
                Color[1] =0.6; //black
                Color[2] = 0.3;
                Matrix::createScaleMat(S,0.8f,0.08f,0.08f);           //scale
                Matrix::createRotateMatZ(R,0.2);
                Matrix::createTranslateMat(T, 0.5f, 0.0f, 0.0f);    //local Translate

                Matrix::createRotateMatZ(Rz,sin(Time)/2.0);
                Matrix::multMat(Temp,T,S);
                Matrix::multMat(Temp2,R, Temp);
                Matrix::multMat(Temp,Rz, Temp2);
                Matrix::createTranslateMat(T, 1.0f, 0.1f, 0.2f);    //local Translate
                Matrix::multMat(Temp2,T,Temp);
                Matrix::multMat(Temp,Rg,Temp2);
                Matrix::multMat(MV, Tg, Temp);
		prog->bind();
                glUniform3fv(prog->getUniform("Color"),1,Color);
		glUniformMatrix4fv(prog->getUniform("P"), 1, GL_FALSE, P);
		glUniformMatrix4fv(prog->getUniform("MV"), 1, GL_FALSE, MV);
		shape->draw(prog);
		prog->unbind();
                //cylinder
		//send the matrices to the shaders
                Matrix::createScaleMat(S,1.5f,1.5f,0.9f);           //scale
                Matrix::createTranslateMat(T, 0.0f, 2.6f, 0.0f);    //local Translate
                Matrix::createRotateMatX(R,M_PI/2.0);
                Matrix::multMat(Temp,R,S);
                Matrix::multMat(Temp2,T, Temp);
                Matrix::multMat(Temp,Rg,Temp2);
                Matrix::multMat(MV, Tg, Temp);

                prog3->bind();
		glUniformMatrix4fv(prog3->getUniform("P"), 1, GL_FALSE, P);
		glUniformMatrix4fv(prog3->getUniform("MV"), 1, GL_FALSE, MV);
                glBindVertexArray(Cylinder_vertex);
                glDrawElements(GL_TRIANGLES, 240, GL_UNSIGNED_INT, 0);
		glBindVertexArray(0);

		prog3->unbind();

                //cylinder
		//send the matrices to the shaders
                Matrix::createScaleMat(S,2.0f,2.0f,0.4);           //scale
                Matrix::createTranslateMat(T, 0.0f, 1.9f, 0.0f);    //local Translate
                Matrix::createRotateMatX(R,M_PI/2.0);
                Matrix::multMat(Temp,R,S);
                Matrix::multMat(Temp2,T, Temp);
                Matrix::multMat(Temp,Rg,Temp2);
                Matrix::multMat(MV, Tg, Temp);

                prog3->bind();
		glUniformMatrix4fv(prog3->getUniform("P"), 1, GL_FALSE, P);
		glUniformMatrix4fv(prog3->getUniform("MV"), 1, GL_FALSE, MV);
                glBindVertexArray(Cylinder_vertex);
                glDrawElements(GL_TRIANGLES, 240, GL_UNSIGNED_INT, 0);
		glBindVertexArray(0);

		prog3->unbind();

                //Ball
                //Ball
                Matrix::createScaleMat(S,4.5f,4.5f,4.5f);           //scale
                Matrix::createTranslateMat(T, 0.0f, 0.0f, 0.0f);    //local Translate
                Matrix::multMat(Temp2,T, S);
                Matrix::multMat(Temp,Rg,Temp2);
                Matrix::multMat(MV, Tg, Temp);
		prog2->bind();
		glUniformMatrix4fv(prog2->getUniform("P"), 1, GL_FALSE, P);
		glUniformMatrix4fv(prog2->getUniform("MV"), 1, GL_FALSE, MV);
		shape->draw(prog2);
		prog2->unbind();

                //snow
                Color[0] = 1.0;
                Color[1] = 1.0;
                Color[2] = 1.0;

                Matrix::createScaleMat(S,0.2f,0.2f,0.2f);           //scale
                Matrix::createTranslateMat(T, -1.9f, 1.8f, 1.7f);    //local Translate
                Matrix::createRotateMatZ(Rz,Time/2.0);
                Matrix::multMat(Temp2,T, S);
                Matrix::multMat(Temp,Rz,Temp2);
                Matrix::multMat(Temp2,Rg,Temp);
                Matrix::multMat(MV, Tg, Temp2);

		prog4->bind();
		glUniformMatrix4fv(prog4->getUniform("P"), 1, GL_FALSE, P);
		glUniformMatrix4fv(prog4->getUniform("MV"), 1, GL_FALSE, MV);
                glUniform3fv(prog->getUniform("Color"),1,Color);
		shape->draw(prog4);
		prog->unbind();

                Matrix::createScaleMat(S,0.15f,0.15f,0.15f);           //scale
                Matrix::createTranslateMat(T, 1.8f, 1.7f, 1.9f);    //local Translate
                Matrix::createRotateMatZ(Rz,-(Time/2.5));
                Matrix::multMat(Temp2,T, S);
                Matrix::multMat(Temp,Rz,Temp2);
                Matrix::multMat(Temp2,Rg,Temp);
                Matrix::multMat(MV, Tg, Temp2);
		prog4->bind();
		glUniformMatrix4fv(prog4->getUniform("P"), 1, GL_FALSE, P);
		glUniformMatrix4fv(prog4->getUniform("MV"), 1, GL_FALSE, MV);
                glUniform3fv(prog->getUniform("Color"),1,Color);
		shape->draw(prog4);
		prog->unbind();

                Matrix::createScaleMat(S,0.15f,0.15f,0.15f);           //scale
                Matrix::createTranslateMat(T, 1.8f, 1.7f, 1.9f);    //local Translate
                Matrix::createRotateMatZ(Rz,(Time/2.6));
                Matrix::multMat(Temp2,T, S);
                Matrix::multMat(Temp,Rz,Temp2);
                Matrix::multMat(Temp2,Rg,Temp);
                Matrix::multMat(MV, Tg, Temp2);
		prog4->bind();
		glUniformMatrix4fv(prog4->getUniform("P"), 1, GL_FALSE, P);
		glUniformMatrix4fv(prog4->getUniform("MV"), 1, GL_FALSE, MV);
                glUniform3fv(prog->getUniform("Color"),1,Color);
		shape->draw(prog4);
		prog->unbind();


		

		
                //glUniform1f(prog2->getUniform("Time"),Time);
                //float Time = glfwGetTime();

		

		
                //glUniform1f(prog2->getUniform("Time"),Time);
                //float Time = glfwGetTime();
                /*Matrix::createTranslateMat(T, -1.0f, 0.0f, 0.0f);    //local Translate
                Matrix::multMat(Temp1,T, S);
                Matrix::multMat(Temp2,Rg,Temp1);
                Matrix::multMat(MV,Tg,Temp2);
		// Draw mesh using GLSL
		prog->bind();
		glUniformMatrix4fv(prog->getUniform("P"), 1, GL_FALSE, P);
		glUniformMatrix4fv(prog->getUniform("MV"), 1, GL_FALSE, MV);
		shape->draw(prog);
		prog->unbind();

                //"H""|"
                Matrix::createTranslateMat(T, -3.0f, 0.0f, 0.0f);    //local Translate
                Matrix::multMat(Temp1,T, S);
                Matrix::multMat(Temp2,Rg,Temp1);
                Matrix::multMat(MV,Tg,Temp2);
		// Draw mesh using GLSL
		prog->bind();
		glUniformMatrix4fv(prog->getUniform("P"), 1, GL_FALSE, P);
		glUniformMatrix4fv(prog->getUniform("MV"), 1, GL_FALSE, MV);
		shape->draw(prog);
		prog->unbind();

		// Draw mesh using GLSL   "H""-"
                Matrix::createScaleMat(S,1.2f,0.5f,1.0f);           //scale
                Matrix::createTranslateMat(T, -2.0f, 0.0f, 0.0f);    //local Translate
                Matrix::multMat(Temp1,T, S);
                Matrix::multMat(Temp2,Rg,Temp1);
                Matrix::multMat(MV,Tg,Temp2);
		prog->bind();
		glUniformMatrix4fv(prog->getUniform("P"), 1, GL_FALSE, P);
		glUniformMatrix4fv(prog->getUniform("MV"), 1, GL_FALSE, MV);
		shape->draw(prog);
		prog->unbind();*/

	}
};

int main(int argc, char **argv)
{
	// Where the resources are loaded from
	std::string resourceDir = "../resources";

	if (argc >= 2)
	{
		resourceDir = argv[1];
	}

	Application *application = new Application();

	// Your main will always include a similar set up to establish your window
	// and GL context, etc.

	WindowManager *windowManager = new WindowManager();
	windowManager->init(640, 480);
	windowManager->setEventCallbacks(application);
	application->windowManager = windowManager;

	// This is the code that will likely change program to program as you
	// may need to initialize or set up different data and state

	application->init(resourceDir);
	application->initGeom(resourceDir);

	// Loop until the user closes the window.
	while (! glfwWindowShouldClose(windowManager->getHandle()))
	{
		// Render scene.
		application->render();

		// Swap front and back buffers.
		glfwSwapBuffers(windowManager->getHandle());
		// Poll for and process events.
		glfwPollEvents();
	}

	// Quit program.
	windowManager->shutdown();
	return 0;
}
