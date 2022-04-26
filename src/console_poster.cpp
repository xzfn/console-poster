#include <iostream>
#include <memory>

#include <FL/Fl.H>
#include <FL/Fl_Native_File_Chooser.H>

#include "ui.h"

#ifdef WIN32
#include <FL/x.H>
#endif

#include "console_poster_core.h"


bool CONFIG_CLICK_SEND = false;


class ConsolePosterApp {
public:
    ConsolePosterApp()
        :m_ui(new Ui())
    {
        setup();
    }

    void setup() {
        set_icon();
        m_ui->m_title->value("C:\\WINDOWS\\system32\\cmd.exe");
        m_ui->m_list->type(FL_HOLD_BROWSER);
        m_ui->m_list->callback(
            call_proxy<ConsolePosterApp, &ConsolePosterApp::on_select>, this
        );
        m_ui->m_load->callback(
            call_proxy<ConsolePosterApp, &ConsolePosterApp::on_load>, this
        );
    }

    int run() {
        m_ui->m_window->show();
        return Fl::run();
    }

    ConsolePosterApp(const ConsolePosterApp&) = delete;
    ConsolePosterApp& operator=(const ConsolePosterApp&) = delete;

private:
	template<typename T, void(T::*mf)()>
	static void call_proxy(Fl_Widget* w, void* data) {
		ConsolePosterApp* app = (ConsolePosterApp*)data;
		(app->*mf)();
	}

private:
    void on_load() {
        std::cout << "on_load\n";
        Fl_Native_File_Chooser file_chooser;
        file_chooser.title("Pick a text file");
        file_chooser.type(Fl_Native_File_Chooser::BROWSE_FILE);
        file_chooser.filter("Text\t*.txt");
        switch (file_chooser.show()) {
        case -1:
            std::cout << "ERROR: " << file_chooser.errmsg() << "\n";
            break;
        case 1:
            std::cout << "cancelled\n";
            break;
        default:
            const char* file_utf8 = file_chooser.filename();
            load_file(file_utf8);
        }
    }

    void load_file(const char* file_utf8) {
        m_ui->m_list->load(file_utf8);
    }

    void on_select() {
        std::cout << "on_select" << "\n";
        int clicks = Fl::event_clicks();
        bool should_send = false;
        if (CONFIG_CLICK_SEND) {
            if (clicks == 0) {
                should_send = true;
            }
        }
        else {
            if (clicks == 1) {
                should_send = true;
            }
        }

        std::cout << m_ui->m_list->value() << ' ' << Fl::event_clicks() << "\n";
        std::string target = m_ui->m_title->value();
        if (should_send) {
            std::vector<HWND> hwnds = find_hwnds_by_title(target);
            for (HWND hwnd : hwnds) {
                std::cout << "Found " << hwnd << '\n';
            }
            if (!hwnds.empty()) {
                HWND hwnd = hwnds[0];
                const char* text = m_ui->m_list->text(m_ui->m_list->value());
                std::cout << "Send " << text << '\n';
                send_string(hwnd, text, VK_RETURN);
            }
        }
    }

	void set_icon() {
#ifdef WIN32
		m_ui->m_window->icon(LoadIcon(fl_display, MAKEINTRESOURCE(107)));
#endif
	}

private:
    std::unique_ptr<Ui> m_ui;
};

int main() {
    std::cout << "Console Poster by wangyueheng\n";
    std::cout << "Source code: " << "https://github.com/xzfn/console-poster" << "\n";

    ConsolePosterApp app;
    return app.run();
}
