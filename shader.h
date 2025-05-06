#pragma once	// Ϊ�˱���ͬһ��ͷ�ļ���������include�����
#include<string>
#include<fstream>
#include<sstream>
#include<iostream>
using namespace std;
#include <glad/glad.h>
#include <GLFW/glfw3.h>

// �����Լ�����ɫ��
class Shader
{
private:
	GLuint vertex, fragment;	// ������ɫ�� �� ƬԪ��ɫ�� 
public:
	GLuint Program;				// ��ɫ�������ID

	// Constructor(��ɫ�����캯��)
	Shader(const GLchar* vertexPath, const GLchar* fragmentPath)
	{
		// �ļ���ȡϵ�еı�������
		string vertexCode;
		string fragmentCode;
		ifstream vShaderFile;
		ifstream fShaderFile;

		// �쳣���ƴ�����֤ifstream��������׳��쳣��
		vShaderFile.exceptions(ifstream::badbit);
		fShaderFile.exceptions(ifstream::badbit);

		try
		{
			// ���ļ�
			vShaderFile.open(vertexPath);
			fShaderFile.open(fragmentPath);
			stringstream vShaderStream, fShaderStream;

			// ��ȡ�ļ��Ļ������ݵ���������
			vShaderStream << vShaderFile.rdbuf();
			fShaderStream << fShaderFile.rdbuf();

			// �ر��ļ�������
			vShaderFile.close();
			fShaderFile.close();

			// ת����������string
			vertexCode = vShaderStream.str();
			fragmentCode = fShaderStream.str();

		}
		catch (ifstream::failure e) {	// �����쳣ʱ���
			cout << "ERROR::SHADER::FILE_NOT_SUCCESSFULLY_READ" << endl;
		}

		/* �� string ���͵��ַ���ת��Ϊ char���� ���� */
		const GLchar* vShaderCode = vertexCode.c_str();
		const GLchar* fShaderCode = fragmentCode.c_str();

		/* ������ɫ�� */
		this->vertex = glCreateShader(GL_VERTEX_SHADER);				// ����������ɫ������
		glShaderSource(this->vertex, 1, &vShaderCode, NULL);			// ��������ɫ�������ݴ�����
		glCompileShader(this->vertex);								// ���붥����ɫ��
		GLint flag;												// �����жϱ����Ƿ�ɹ�
		GLchar infoLog[512];
		glGetShaderiv(this->vertex, GL_COMPILE_STATUS, &flag); // ��ȡ����״̬
		if (!flag)
		{
			glGetShaderInfoLog(this->vertex, 512, NULL, infoLog);
			cout << "ERROR::SHADER::VERTEX::COMPILATION_FAILED\n" << infoLog << endl;
		}

		/* ƬԪ��ɫ�� */
		this->fragment = glCreateShader(GL_FRAGMENT_SHADER);			// ����ƬԪ��ɫ������
		glShaderSource(this->fragment, 1, &fShaderCode, NULL);		// ��ƬԪ��ɫ�������ݴ�����
		glCompileShader(this->fragment);								// ���붥����ɫ��
		glGetShaderiv(this->fragment, GL_COMPILE_STATUS, &flag);		// ��ȡ����״̬
		if (!flag)
		{
			glGetShaderInfoLog(this->fragment, 512, NULL, infoLog);
			cout << "ERROR::SHADER::FRAGMENT::COMPILATION_FAILED\n" << infoLog << endl;
		}

		/* ��ɫ������ */
		this->Program = glCreateProgram();
		glAttachShader(this->Program, this->vertex);
		glAttachShader(this->Program, this->fragment);
		glLinkProgram(this->Program);
		if (!flag)
		{
			glGetProgramInfoLog(this->Program, 512, NULL, infoLog);
			cout << "ERROR::SHADER::PROGRAM::LINKING_FAILED\n" << infoLog << endl;
		}
		// ɾ����ɫ���������Ѿ����ӵ����ǵĳ������ˣ��Ѿ�������Ҫ��
		glDeleteShader(this->vertex);
		glDeleteShader(this->fragment);
	}

	// Deconstructor(��������)
	~Shader()
	{
		glDetachShader(this->Program, this->vertex);
		glDetachShader(this->Program, this->fragment);
		glDeleteShader(this->vertex);
		glDeleteShader(this->fragment);
		glDeleteProgram(this->Program);
	}

	void Use()
	{
		glUseProgram(this->Program);
	}
};
