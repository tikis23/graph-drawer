#pragma once

#include <GL/glew.h>
#include <iostream>
#include <string>
#include <sstream>	
#include <fstream>

class Shader
{
public:
	
	void LoadShaders(const char* name, const char* VertexPath, const char* FragmentPath, const char* geometryPath = NULL)
	{
		std::string vertexCode;
		std::string fragmentCode;
		std::ifstream vertexShaderFile;
		std::ifstream fragmentShaderFile;
		
		vertexShaderFile.exceptions(std::ifstream::failbit | std::ifstream::badbit);
		fragmentShaderFile.exceptions(std::ifstream::failbit | std::ifstream::badbit);
		//load
		try
		{
			std::stringstream vertexShaderStream;
			std::stringstream fragmentShaderStream;

			vertexShaderFile.open(VertexPath);
			fragmentShaderFile.open(FragmentPath);
			vertexShaderStream << vertexShaderFile.rdbuf();
			fragmentShaderStream << fragmentShaderFile.rdbuf();
			vertexShaderFile.close();
			fragmentShaderFile.close();
			vertexCode = vertexShaderStream.str();
			fragmentCode = fragmentShaderStream.str();
		}
		catch(std::ifstream::failure e)
		{
			std::cout << "ERROR::SHADER::FILE_NOT_SUCCESFULLY_READ" << std::endl;
		}
		const char* vertexShaderCode = vertexCode.c_str();
		const char* fragmentShaderCode = fragmentCode.c_str();

		unsigned int vertex;
		unsigned int fragment;

		//vertex
		vertex = glCreateShader(GL_VERTEX_SHADER);
		glShaderSource(vertex, 1, &vertexShaderCode, NULL);
		glCompileShader(vertex);
		checkCompileErrors(vertex, "VERTEX");

		//fragment
		fragment = glCreateShader(GL_FRAGMENT_SHADER);
		glShaderSource(fragment, 1, &fragmentShaderCode, NULL);
		glCompileShader(fragment);
		checkCompileErrors(fragment, "FRAGMENT");

		//geometry
		if (geometryPath != NULL)
		{
			std::string geometryCode;
			std::ifstream geometryShaderFile;

			geometryShaderFile.exceptions(std::ifstream::failbit | std::ifstream::badbit);
			//load
			try
			{
				std::stringstream geometryShaderStream;

				geometryShaderFile.open(geometryPath);
				geometryShaderStream << geometryShaderFile.rdbuf();
				geometryShaderFile.close();
				geometryCode = geometryShaderStream.str();
			}
			catch (std::ifstream::failure e)
			{
				std::cout << "ERROR::SHADER::FILE_NOT_SUCCESFULLY_READ" << std::endl;
			}
			const char* geometryShaderCode = geometryCode.c_str();

			unsigned int geometry;
			geometry = glCreateShader(GL_GEOMETRY_SHADER);
			glShaderSource(geometry, 1, &geometryShaderCode, NULL);
			glCompileShader(geometry);
			checkCompileErrors(geometry, "GEOMETRY");

			ID = glCreateProgram();
			glAttachShader(ID, vertex);
			glAttachShader(ID, geometry);
			glAttachShader(ID, fragment);
			glLinkProgram(ID);
			checkCompileErrors(ID, name);
			glDeleteShader(vertex);
			glDeleteShader(geometry);
			glDeleteShader(fragment);
		}
		else
		{
			ID = glCreateProgram();
			glAttachShader(ID, vertex);
			glAttachShader(ID, fragment);
			glLinkProgram(ID);
			checkCompileErrors(ID, name);
			glDeleteShader(vertex);
			glDeleteShader(fragment);
		}
	}

	void Use()
	{
		glUseProgram(ID);
	}

	// float
	void setUniform1f(const std::string& location, float value_1) const
	{
		glUniform1f(glGetUniformLocation(ID, location.c_str()), value_1);
	}
	void setUniform2f(const std::string& location, float value_1, float value_2) const
	{
		glUniform2f(glGetUniformLocation(ID, location.c_str()), value_1, value_2);
	}
	void setUniform3f(const std::string& location, float value_1, float value_2, float value_3) const
	{
		glUniform3f(glGetUniformLocation(ID, location.c_str()), value_1, value_2, value_3);
	}
	void setUniform4f(const std::string& location, float value_1, float value_2, float value_3, float value_4) const
	{
		glUniform4f(glGetUniformLocation(ID, location.c_str()), value_1, value_2, value_3, value_4);
	}

	// int
	void setUniform1i(const std::string& location, int value_1) const
	{
		glUniform1i(glGetUniformLocation(ID, location.c_str()), value_1);
	}
	void setUniform2i(const std::string& location, int value_1, int value_2) const
	{
		glUniform2i(glGetUniformLocation(ID, location.c_str()), value_1, value_2);
	}
	void setUniform3i(const std::string& location, int value_1, int value_2, int value_3) const
	{
		glUniform3i(glGetUniformLocation(ID, location.c_str()), value_1, value_2, value_3);
	}
	void setUniform4i(const std::string& location, int value_1, int value_2, int value_3, int value_4) const
	{
		glUniform4i(glGetUniformLocation(ID, location.c_str()), value_1, value_2, value_3, value_4);
	}

	// arrays
	void setUniform1fv(const std::string& location, int size, float value[]) const
	{
		glUniform1fv(glGetUniformLocation(ID, location.c_str()), size, value);
	}
	void setUniform1iv(const std::string& location, int size, int value[]) const
	{
		glUniform1iv(glGetUniformLocation(ID, location.c_str()), size, value);
	}

	// matrix
	void setUniformMatrix4fv(const std::string& location, int size, bool transpose, float value[]) const
	{
		glUniformMatrix4fv(glGetUniformLocation(ID, location.c_str()), size, transpose, value);
	}

	unsigned int ID;
private:

	void checkCompileErrors(unsigned int shader, std::string type)
	{
		int success;
		char infoLog[1024];
		if (type != "PROGRAM")
		{
			glGetShaderiv(shader, GL_COMPILE_STATUS, &success);
			if (!success)
			{
				glGetShaderInfoLog(shader, 1024, NULL, infoLog);

				std::cout << "ERROR::SHADER_COMPILATION_ERROR of type: " << type << "\n" << infoLog <<
					"\n -- --------------------------------------------------- -- " << std::endl;
			}
		}
		else
		{
			glGetProgramiv(shader, GL_LINK_STATUS, &success);
			if (!success)
			{
				glGetProgramInfoLog(shader, 1024, NULL, infoLog);

				std::cout << "ERROR::PROGRAM_LINKING_ERROR of type: " << type << "\n" << infoLog <<
					"\n -- --------------------------------------------------- -- " << std::endl;
			}
		}
	}
};