: verify   = "*>^~64.4"
: tag      = "miiskira.render"
: version  = "miiskira"
: author   = "vya"

["shader" shader]
	[shader "vert.background" vert]
		"shader" @"vertex" => &"vertex.vert.spv"
		"input" @string => "vertex-p3c3"
		"output" @string => "inner-c4"
		[vert "uniform" vert-uni]
			[vert-uni "0"]
				"layout" @string => "viewport"
				"share-model" @null
				"share-pipe" @null
	[shader "frag.background" frag]
		"shader" @"fragment" => &"fragment.frag.spv"
		"input" @string => "inner-c4"
		"output" @string => "inner-c4"
