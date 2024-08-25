local core = require("ssl")

screen_width, screen_height = get_screen_dimensions()
disable_screen_resize()

load_texture('logo', './logo.png')

function pre_init_load()
	
end
function pre_init_unload()
	
end
function pre_init_update()
	
end
function pre_init_render()
	render_texture("logo", screen_width/2, screen_height/2, 256, 256, true, 0)
end

register_state('none', 'pre_init')

set_state('pre_init')