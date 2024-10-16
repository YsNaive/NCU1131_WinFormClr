#pragma once

#include "Core.h"
namespace CppCLRWinFormsProject {

	public ref class Window : public System::Windows::Forms::Form
	{
	private: System::Windows::Forms::Timer^ timer_update;
	private: System::Windows::Forms::Timer^ timer_second;
	public:
		static Window^ instance;
		Graphics^ graphics;
		Window(void)
		{
			instance = this;
			InitializeComponent();
			this->FormBorderStyle = Windows::Forms::FormBorderStyle::Sizable;
			this->DoubleBuffered  = true; 
			this->KeyDown += gcnew KeyEventHandler(this, &Window::OnKeyDown);
			this->KeyUp   += gcnew KeyEventHandler(this, &Window::OnKeyUp);

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
			this->timer_second = (gcnew System::Windows::Forms::Timer(this->components));
			this->SuspendLayout();
			// 
			// timer_update
			// 
			this->timer_update->Enabled = true;
			this->timer_update->Interval = 25;
			this->timer_update->Tick += gcnew System::EventHandler(this, &Window::timer_update_Tick);
			// 
			// timer_second
			// 
			this->timer_second->Enabled = true;
			this->timer_second->Interval = 1000;
			this->timer_second->Tick += gcnew System::EventHandler(this, &Window::timer_second_Tick);
			// 
			// Window
			// 
			this->AutoScaleDimensions = System::Drawing::SizeF(11, 23);
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
		if (!getKey.count(e->KeyValue)) {
			getKeyDown.insert(e->KeyValue);
		}
		getKey.insert(e->KeyValue);
	}

	private: virtual void OnKeyUp(Object^ sender, KeyEventArgs^ e)
	{
		getKey.erase(e->KeyValue); 
		getKeyUp.insert(e->KeyValue);
	}

	private: System::Void timer_update_Tick(System::Object^ sender, System::EventArgs^ e)
	{
		PreUpdate::Invoke();
		Update::Invoke();
		LateUpdate::Invoke();
	}
	
	private: System::Void timer_second_Tick(System::Object^ sender, System::EventArgs^ e) {
	}
	};
}
