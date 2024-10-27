#pragma once

#include "Core.h"

namespace CppCLRWinFormsProject {

	public ref class Window : public System::Windows::Forms::Form
	{
	private: System::Windows::Forms::Timer^ timer_update;

	public:
		static Window^ instance;
		Graphics^ graphics;
		Window(void)
		{
			instance = this;
			InitializeComponent();
			this->FormBorderStyle = Windows::Forms::FormBorderStyle::Sizable;
			this->DoubleBuffered  = true; 
			this->KeyDown    += gcnew KeyEventHandler  (this, &Window::OnKeyDown);
			this->KeyUp      += gcnew KeyEventHandler  (this, &Window::OnKeyUp);
			this->MouseWheel += gcnew MouseEventHandler(this, &Window::OnMouseWheel);
			this->MouseDown  += gcnew MouseEventHandler(this, &Window::OnMouseDown);
			this->MouseUp    += gcnew MouseEventHandler(this, &Window::OnMouseUp);
			Start::Invoke();
		}

	protected:
		~Window()
		{
			if (components)
			{
				delete components;
			}
		}

	protected:
	private: System::ComponentModel::IContainer^ components;

	private:
		/// <summary>
		/// Required designer variable.
		/// </summary>


#pragma region Windows Form Designer generated code
		/// <summary>
		/// Required method for Designer support - do not modify
		/// the contents of this method with the code editor.
		/// </summary>
		void InitializeComponent(void)
		{
			this->components = (gcnew System::ComponentModel::Container());
			this->timer_update = (gcnew System::Windows::Forms::Timer(this->components));
			this->SuspendLayout();
			// 
			// timer_update
			// 
			this->timer_update->Enabled = true;
			this->timer_update->Interval = 25;
			this->timer_update->Tick += gcnew System::EventHandler(this, &Window::timer_update_Tick);
			// 
			// Window
			// 
			this->AutoScaleDimensions = System::Drawing::SizeF(9, 19);
			this->AutoScaleMode = System::Windows::Forms::AutoScaleMode::Font;
			this->ClientSize = System::Drawing::Size(582, 353);
			this->Font = (gcnew System::Drawing::Font(L"Consolas", 12));
			this->Margin = System::Windows::Forms::Padding(4, 5, 4, 5);
			this->Name = L"Window";
			this->Text = L"Form1";
			this->ResumeLayout(false);

		}
#pragma endregion

	protected: virtual void OnPaint(System::Windows::Forms::PaintEventArgs^ e) override
	{
		graphics = e->Graphics;
		OnPaint::Invoke();
	}

	private: virtual void OnKeyDown(Object^ sender, KeyEventArgs^ e)
	{
		if (!Input::s_getKey.count(e->KeyValue)) {
			Input::s_getKeyDown.insert(e->KeyValue);
		}
		Input::s_getKey.insert(e->KeyValue);
	}

	private: virtual void OnKeyUp(Object^ sender, KeyEventArgs^ e)
	{
		Input::s_getKey.erase(e->KeyValue); 
		Input::s_getKeyUp.insert(e->KeyValue);
	}

	void OnMouseDown(Object^ sender, MouseEventArgs^ e)
	{
		using MouseButtons = System::Windows::Forms::MouseButtons;
		Input::s_mouseState[(e->Button == MouseButtons::Left) ? 0 : 1][0] = true;
		Input::s_mouseState[(e->Button == MouseButtons::Left) ? 0 : 1][1] = true;
	}

	void OnMouseUp(Object^ sender, MouseEventArgs^ e)
	{
		using MouseButtons = System::Windows::Forms::MouseButtons;
		Input::s_mouseState[(e->Button == MouseButtons::Left) ? 0 : 1][1] = false;
		Input::s_mouseState[(e->Button == MouseButtons::Left) ? 0 : 1][2] = true;
	}

	void OnMouseWheel(Object^ sender, MouseEventArgs^ e)
	{
		Input::MouseScroller = e->Delta;
	}

	private: System::Void timer_update_Tick(System::Object^ sender, System::EventArgs^ e)
	{
		PreUpdate::Invoke();
		Update::Invoke();
		LateUpdate::Invoke();
		vector<GameObject*> toDestory;
		for (auto obj : GameObject::GetInstances()) {
			if (obj->is_Destory())
				toDestory.push_back(obj);
		}
		for (auto obj : toDestory)
			delete obj;
		Input::Time += 0.025;
	}
	};
}
