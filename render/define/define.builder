: verify   = "*>^~64.4"
: tag      = "miiskira.render"
: version  = "miiskira"
: author   = "vya"

# layout
["layout" layout]
	# vertex
	[layout "vertex-p3c3"]
		"position" @"vec3" => (f32[3]) {}
		"color" @"vec3" => (f32[3]) {}
	# input output
	[layout "inner-c4"]
		"color" @"vec4" => (f32[4]) {}
	# uniform
	[layout "viewport"]
		"viewport" @"mat4" => (f32[16]) {}

# blend
["blend" blend]
	# default
	[blend "default"]
		[~ "attachment"]
			[~ "-"]
				"src-color" @string => "sa"
				"dst-color" @string => "1-sa"
				"op-color" @string => "+"
				"src-alpha" @string => "1"
				"dst-alpha" @string => "0"
				"op-alpha" @string => "+"

# render
["render" render]
	[render "default" -]
		[- "attachment"]
			[~ "a-color"]
				"format" @string => "b8g8r8a8_unorm"
				"sample" @string => "1"
				"load" @string => "clear"
				"store" @string => "store"
				"stencil_load" @string => "none"
				"stencil_store" @string => "none"
				"initial" @string => "undefined"
				"finally" @string => "khr_present_src"
		[- "subpass"]
			[~ "p-color"]
				"type" @string => "graphics"
				[~ "color"]
					"a-color" @string => "color_attachment_optimal"

