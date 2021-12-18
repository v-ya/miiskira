: verify   = "*>^~64.4"
: tag      = "miiskira.render"
: version  = "miiskira"
: author   = "vya"

["layout" layout]
	[layout "vertex-background"]
		"position" @f32 => (f32[3]) {}
		"color" @f32 => (f32[3]) {}
	[layout "uniform-background"]
		"viewport" @f32 => (f32[16]) { 1, 0, 0, 0,   0, 1, 0, 0,   0, 0, 1, 0,   0, 0, 0, 1 }

["shader" shader]
	"background.vertex" @"vert" => &"vertex.vert.spv"
	"background.fragment" @"frag" => &"fragment.frag.spv"
