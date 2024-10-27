#pragma once
#include "Core.h"

public ref class Window : public System::Windows::Forms::Form
{
public:
	Window(void)
	{
		InitializeComponent();
		this->DoubleBuffered = true;
	}

protected:
	~Window()
	{
		if (components)
		{
			delete components;
		}
	}

private:
	System::ComponentModel::Container^ components;

#pragma region Windows Form Designer generated code
	/// <summary>
	/// Required method for Designer support - do not modify
	/// the contents of this method with the code editor.
	/// </summary>
	void InitializeComponent(void)
	{
		this->components = gcnew System::ComponentModel::Container();
		this->Size = System::Drawing::Size(300, 300);
		this->Text = L"Tank Survivor";
		this->Padding = System::Windows::Forms::Padding(0);
		this->AutoScaleMode = System::Windows::Forms::AutoScaleMode::Font;
	}
#pragma endregion
};