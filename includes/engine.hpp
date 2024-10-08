// define gaurd
#ifndef ENGINE_H_
#define ENGINE_H_
// end define gaurd
#include <string>

#include <SDL3/SDL.h>
#include <SDL3/SDL_thread.h>
#include <SDL3/SDL_image.h>
#include <SDL3/SDL_ttf.h>
#include <SDL3/SDL_mixer.h>

#include <imgui.h>
#include <imgui/imgui_impl_sdl3.h>
#include <imgui/imgui_impl_sdlrenderer3.h>

#include <lua.hpp>


#define TARGET_FPS 60 // How many frames per second are *desired*.
#define TARGET_TPS 30 // How many update ticks per second are *desired*.

#define MAX_TEXTURES 1024
#define MAX_SUBTEXTURES 4096
#define MAX_FONTS 32
#define MAX_AVAILABLE_STATES 256
#define MAX_INPUT_TEXTS 256
class State {
	public:
		std::string state_prefix;
		State() {};
		void render();
		void unload();
		void load();
		void update();
	private:
		virtual void _internal_render();
		virtual void _internal_unload();
		virtual void _internal_load();
		virtual void _internal_update();
};

class LuaState : public State {
	public:
		std::string lua_source;
		LuaState(std::string, std::string);
		LuaState() {} ;
	private:
		void _internal_render();
		void _internal_unload();
		void _internal_load();
		void _internal_update();
};
class state_machine {
public:
    state_machine();
    ~state_machine();

    void render();
    void update();

	void set_current_state(std::string state_id);
	
	std::string statename;
	
	State *states[MAX_AVAILABLE_STATES];
	State *current_state= NULL;

	void register_state(std::string file, std::string name);
};

class texture {
public:
	std::string id;
	SDL_Texture *data = NULL;
	
	int width = 0;
	int height = 0;
	
	~texture();
	texture();
	texture(const char *id, const char *file);
	
	int render_at(int x, int y, double scale_x, double scale_y, bool centered, double rotation);
};

class subtexture {
public:
	std::string id;
	texture *texture_source = NULL;
	
	int source_x = 0;
	int source_y = 0;
	int width = 0;
	int height = 0;
	
	subtexture();
	subtexture(const char *id, const char *source, int source_x, int source_y, int w, int h);
	
	int render_at(int x, int y, double scale_x, double scale_y, bool centered, double rotation);
};

class font {
public:
	const char *id = NULL;
	TTF_Font *data = NULL;
	
	~font();
	font();
	font(const char *id, const char *file, int size);
	
	int render_at(const char *text, int x, int y, int r, int g, int b, bool centered, double rotation);
};

struct keyboard {
public:
	bool pressed[256];
	bool released[256];
	bool down[256];
};

struct mouse {
public:
	bool pressed[4];
	bool released[4];
	bool down[4];
};

class engine {
private:
public:
	Uint32 last_render = 0; // in ms
	Uint32 last_update = 0; // in ms
	Uint32 tick_delta = 0;
	Uint32 frame_delta = 0;
	Uint32 tick_rate_real = 0;
	Uint32 frame_rate_real = 0;
	Uint32 tick_rate_target = TARGET_TPS;
	Uint32 frame_rate_target = TARGET_FPS;
	
	texture *Textures[MAX_TEXTURES];
	subtexture *Subtextures[MAX_SUBTEXTURES];
	font *Fonts[MAX_FONTS];

	SDL_Window *window = NULL;
	lua_State *lua_instance = NULL;
	lua_State *lua_thread = NULL;
	SDL_Renderer *renderer = NULL;
	SDL_Mutex *input_mutex = NULL;
	SDL_Condition *can_update_input = NULL;
	SDL_Condition *can_process_input = NULL;
	state_machine *state_manager = NULL;
	
	ImGuiIO UIIo; 
	char input_texts[MAX_INPUT_TEXTS][64];
	
	struct keyboard key_states;
	struct mouse mouse_states;
	Uint32 current_tick = 0; // the time since the engine began in ms

	int screen_width = 500;
	int screen_height = 500;

	bool initialized = true;
	bool running = false;
	bool update_input = false;
	bool input_handled = false;

	int loop_function(); // main loop function. calls render and update;
	int handle_events(); // main loop function. calls render and update;

	int render_function(); // called every *fps* times per second;
	int update_function(); // called every update tick;

	int initialize(); // set everything up;
	int cleanup(); // clean everything up;

	int begin(); // starts and handles the main loop;
	void stop(); // called to stop the main loop;

	engine(); // kinda just exists;
	
	int load_texture(const char *id,const char *file_name);
	int create_subtexture(const char *id, const char *texture_source, int sx, int sy, int w, int h);
	int render_texture(const char *id, int x, int y, double scale_x, double scale_y, bool centered, double rotation);
	int render_subtexture(const char *id, int x, int y, double scale_x, double scale_y, bool centered, double rotation);
	
	int load_font(const char *id, const char *file_name, int size);
	int render_text(const char *id, const char *text, int x, int y, int r, int g, int b, bool centered, double rotation);
	void render_line(int x1, int y1, int x2, int y2, int r, int g, int b, int a);
	void render_rectangle(int x, int y, int w, int h, int r, int g, int b, int a, bool centered);
	void render_rectangle_border(int x, int y, int w, int h, int r, int g, int b, int a, bool centered);
};

engine* gEngine = NULL;
#endif
