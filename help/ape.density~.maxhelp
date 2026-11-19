{
	"patcher": {
		"fileversion": 1,
		"appversion": {
			"major": 8,
			"minor": 2,
			"revision": 2,
			"architecture": "x64",
			"modernui": 1
		},
		"classnamespace": "box",
		"rect": [
			80.0,
			90.0,
			935.0,
			770.0
		],
		"bglocked": 0,
		"openinpresentation": 0,
		"default_fontsize": 12.0,
		"default_fontface": 0,
		"default_fontname": "Arial",
		"gridonopen": 1,
		"gridsize": [
			15.0,
			15.0
		],
		"gridsnaponopen": 1,
		"objectsnaponopen": 1,
		"statusbarvisible": 2,
		"toolbarvisible": 1,
		"lefttoolbarpinned": 0,
		"toptoolbarpinned": 0,
		"righttoolbarpinned": 0,
		"bottomtoolbarpinned": 0,
		"toolbars_unpinned_last_save": 0,
		"tallnewobj": 0,
		"boxanimatetime": 200,
		"enablehscroll": 1,
		"enablevscroll": 1,
		"devicewidth": 0.0,
		"description": "Granular synthesis on a sound file",
		"digest": "Granulator (apeSoft)",
		"tags": "granular, apeSoft",
		"style": "",
		"subpatcher_template": "",
		"assistshowspatchername": 0,
		"boxes": [
			{
				"box": {
					"id": "obj-23",
					"maxclass": "newobj",
					"numinlets": 1,
					"numoutlets": 1,
					"outlettype": [
						""
					],
					"patching_rect": [
						105.0,
						190.0,
						72.0,
						22.0
					],
					"text": "loadmess 1"
				}
			},
			{
				"box": {
					"fontname": "Arial",
					"fontsize": 12.0,
					"id": "obj-71",
					"maxclass": "newobj",
					"numinlets": 1,
					"numoutlets": 1,
					"outlettype": [
						""
					],
					"patching_rect": [
						385.0,
						190.0,
						79.0,
						22.0
					],
					"text": "loadmess 10"
				}
			},
			{
				"box": {
					"fontname": "Arial",
					"fontsize": 12.0,
					"format": 6,
					"id": "obj-39",
					"maxclass": "flonum",
					"maximum": 2000.0,
					"minimum": 0.0,
					"numinlets": 1,
					"numoutlets": 2,
					"outlettype": [
						"",
						"bang"
					],
					"parameter_enable": 0,
					"patching_rect": [
						385.0,
						218.0,
						50.0,
						22.0
					]
				}
			},
			{
				"box": {
					"fontname": "Arial",
					"fontsize": 12.0,
					"id": "obj-38",
					"maxclass": "message",
					"numinlets": 2,
					"numoutlets": 1,
					"outlettype": [
						""
					],
					"patching_rect": [
						385.0,
						248.0,
						83.0,
						22.0
					],
					"text": "setDensity $1"
				}
			},
			{
				"box": {
					"fontname": "Arial",
					"fontsize": 12.0,
					"id": "obj-24",
					"maxclass": "number",
					"maximum": 64,
					"minimum": 1,
					"numinlets": 1,
					"numoutlets": 2,
					"outlettype": [
						"",
						"bang"
					],
					"parameter_enable": 0,
					"patching_rect": [
						195.0,
						218.0,
						50.0,
						22.0
					]
				}
			},
			{
				"box": {
					"id": "obj-22",
					"maxclass": "toggle",
					"numinlets": 1,
					"numoutlets": 1,
					"outlettype": [
						"int"
					],
					"parameter_enable": 0,
					"patching_rect": [
						295.0,
						218.0,
						20.0,
						20.0
					]
				}
			},
			{
				"box": {
					"fontname": "Arial",
					"fontsize": 12.0,
					"format": 6,
					"id": "obj-21",
					"maxclass": "flonum",
					"numinlets": 1,
					"numoutlets": 2,
					"outlettype": [
						"",
						"bang"
					],
					"parameter_enable": 0,
					"patching_rect": [
						490.0,
						218.0,
						50.0,
						22.0
					]
				}
			},
			{
				"box": {
					"fontname": "Arial",
					"fontsize": 11.0,
					"id": "obj-4",
					"linecount": 2,
					"maxclass": "comment",
					"numinlets": 1,
					"numoutlets": 0,
					"patching_rect": [
						490.0,
						150.0,
						134.0,
						32.0
					],
					"text": "setSamplePosition\nread point, milliseconds"
				}
			},
			{
				"box": {
					"fontname": "Arial",
					"fontsize": 12.0,
					"id": "obj-17",
					"maxclass": "message",
					"numinlets": 2,
					"numoutlets": 1,
					"outlettype": [
						""
					],
					"patching_rect": [
						490.0,
						248.0,
						127.0,
						22.0
					],
					"text": "setSamplePosition $1"
				}
			},
			{
				"box": {
					"fontname": "Arial",
					"fontsize": 11.0,
					"id": "obj-2",
					"linecount": 2,
					"maxclass": "comment",
					"numinlets": 1,
					"numoutlets": 0,
					"patching_rect": [
						630.0,
						150.0,
						89.0,
						32.0
					],
					"text": "setPhase\nread point, 0-1"
				}
			},
			{
				"box": {
					"fontname": "Arial",
					"fontsize": 12.0,
					"format": 6,
					"id": "obj-14",
					"maxclass": "flonum",
					"maximum": 1.0,
					"minimum": 0.0,
					"numinlets": 1,
					"numoutlets": 2,
					"outlettype": [
						"",
						"bang"
					],
					"parameter_enable": 0,
					"patching_rect": [
						630.0,
						218.0,
						50.0,
						22.0
					]
				}
			},
			{
				"box": {
					"fontname": "Arial",
					"fontsize": 12.0,
					"id": "obj-16",
					"maxclass": "message",
					"numinlets": 2,
					"numoutlets": 1,
					"outlettype": [
						""
					],
					"patching_rect": [
						630.0,
						248.0,
						77.0,
						22.0
					],
					"text": "setPhase $1"
				}
			},
			{
				"box": {
					"fontname": "Arial",
					"fontsize": 12.0,
					"id": "obj-6",
					"maxclass": "message",
					"numinlets": 2,
					"numoutlets": 1,
					"outlettype": [
						""
					],
					"patching_rect": [
						110.0,
						606.0,
						50.0,
						22.0
					],
					"text": "replace"
				}
			},
			{
				"box": {
					"fontname": "Arial",
					"fontsize": 12.0,
					"id": "obj-115",
					"maxclass": "newobj",
					"numinlets": 2,
					"numoutlets": 0,
					"patcher": {
						"fileversion": 1,
						"appversion": {
							"major": 8,
							"minor": 2,
							"revision": 2,
							"architecture": "x64",
							"modernui": 1
						},
						"classnamespace": "box",
						"rect": [
							96.0,
							145.0,
							640.0,
							480.0
						],
						"bglocked": 0,
						"openinpresentation": 0,
						"default_fontsize": 12.0,
						"default_fontface": 0,
						"default_fontname": "Arial",
						"gridonopen": 1,
						"gridsize": [
							15.0,
							15.0
						],
						"gridsnaponopen": 1,
						"objectsnaponopen": 1,
						"statusbarvisible": 2,
						"toolbarvisible": 1,
						"lefttoolbarpinned": 0,
						"toptoolbarpinned": 0,
						"righttoolbarpinned": 0,
						"bottomtoolbarpinned": 0,
						"toolbars_unpinned_last_save": 0,
						"tallnewobj": 0,
						"boxanimatetime": 200,
						"enablehscroll": 1,
						"enablevscroll": 1,
						"devicewidth": 0.0,
						"description": "",
						"digest": "",
						"tags": "",
						"style": "",
						"subpatcher_template": "",
						"assistshowspatchername": 0,
						"boxes": [
							{
								"box": {
									"comment": "",
									"id": "obj-10",
									"index": 1,
									"maxclass": "inlet",
									"numinlets": 0,
									"numoutlets": 1,
									"outlettype": [
										""
									],
									"patching_rect": [
										38.0,
										9.0,
										25.0,
										25.0
									]
								}
							},
							{
								"box": {
									"comment": "",
									"id": "obj-7",
									"index": 2,
									"maxclass": "inlet",
									"numinlets": 0,
									"numoutlets": 1,
									"outlettype": [
										""
									],
									"patching_rect": [
										399.0,
										81.0,
										25.0,
										25.0
									]
								}
							},
							{
								"box": {
									"fontname": "Arial",
									"fontsize": 12.0,
									"id": "obj-9",
									"maxclass": "newobj",
									"numinlets": 3,
									"numoutlets": 1,
									"outlettype": [
										"int"
									],
									"patching_rect": [
										407.0,
										294.0,
										94.0,
										20.0
									],
									"text": "drunk 1000 300"
								}
							},
							{
								"box": {
									"id": "obj-8",
									"maxclass": "toggle",
									"numinlets": 1,
									"numoutlets": 1,
									"outlettype": [
										"int"
									],
									"parameter_enable": 0,
									"patching_rect": [
										419.0,
										201.0,
										20.0,
										20.0
									]
								}
							},
							{
								"box": {
									"fontname": "Arial",
									"fontsize": 12.0,
									"id": "obj-1",
									"maxclass": "newobj",
									"numinlets": 2,
									"numoutlets": 1,
									"outlettype": [
										"bang"
									],
									"patching_rect": [
										427.0,
										248.0,
										58.0,
										20.0
									],
									"text": "metro 10"
								}
							},
							{
								"box": {
									"fontname": "Arial",
									"fontsize": 12.0,
									"id": "obj-2",
									"maxclass": "number",
									"numinlets": 1,
									"numoutlets": 2,
									"outlettype": [
										"",
										"bang"
									],
									"parameter_enable": 0,
									"patching_rect": [
										405.0,
										319.0,
										106.0,
										20.0
									]
								}
							},
							{
								"box": {
									"fontname": "Arial",
									"fontsize": 12.0,
									"id": "obj-6",
									"maxclass": "newobj",
									"numinlets": 1,
									"numoutlets": 3,
									"outlettype": [
										"bang",
										"int",
										"int"
									],
									"patching_rect": [
										38.0,
										74.0,
										154.0,
										20.0
									],
									"text": "trigger b i i"
								}
							},
							{
								"box": {
									"fontname": "Arial",
									"fontsize": 12.0,
									"id": "obj-5",
									"maxclass": "newobj",
									"numinlets": 1,
									"numoutlets": 1,
									"outlettype": [
										""
									],
									"patching_rect": [
										38.0,
										46.0,
										85.0,
										20.0
									],
									"text": "loadmess 512"
								}
							},
							{
								"box": {
									"fontname": "Arial",
									"fontsize": 12.0,
									"id": "obj-4",
									"maxclass": "newobj",
									"numinlets": 2,
									"numoutlets": 2,
									"outlettype": [
										"int",
										""
									],
									"patcher": {
										"fileversion": 1,
										"appversion": {
											"major": 8,
											"minor": 2,
											"revision": 2,
											"architecture": "x64",
											"modernui": 1
										},
										"classnamespace": "box",
										"rect": [
											50.0,
											94.0,
											640.0,
											480.0
										],
										"bglocked": 0,
										"openinpresentation": 0,
										"default_fontsize": 12.0,
										"default_fontface": 0,
										"default_fontname": "Arial",
										"gridonopen": 1,
										"gridsize": [
											15.0,
											15.0
										],
										"gridsnaponopen": 1,
										"objectsnaponopen": 1,
										"statusbarvisible": 2,
										"toolbarvisible": 1,
										"lefttoolbarpinned": 0,
										"toptoolbarpinned": 0,
										"righttoolbarpinned": 0,
										"bottomtoolbarpinned": 0,
										"toolbars_unpinned_last_save": 0,
										"tallnewobj": 0,
										"boxanimatetime": 200,
										"enablehscroll": 1,
										"enablevscroll": 1,
										"devicewidth": 0.0,
										"description": "",
										"digest": "",
										"tags": "",
										"style": "",
										"subpatcher_template": "",
										"assistshowspatchername": 0,
										"boxes": [
											{
												"box": {
													"fontname": "Courier New",
													"fontsize": 9.0,
													"id": "obj-1",
													"maxclass": "newobj",
													"numinlets": 2,
													"numoutlets": 1,
													"outlettype": [
														"int"
													],
													"patching_rect": [
														239.0,
														55.0,
														37.0,
														17.0
													],
													"text": "i #1"
												}
											},
											{
												"box": {
													"fontname": "Courier New",
													"fontsize": 9.0,
													"id": "obj-2",
													"linecount": 2,
													"maxclass": "newobj",
													"numinlets": 2,
													"numoutlets": 1,
													"outlettype": [
														""
													],
													"patching_rect": [
														103.0,
														136.0,
														235.0,
														27.0
													],
													"text": "expr 0.5-0.5*cos($i1*3.141592654*2/($i2-1))"
												}
											},
											{
												"box": {
													"fontname": "Courier New",
													"fontsize": 9.0,
													"id": "obj-3",
													"maxclass": "newobj",
													"numinlets": 1,
													"numoutlets": 1,
													"outlettype": [
														"bang"
													],
													"patching_rect": [
														239.0,
														36.0,
														53.0,
														17.0
													],
													"text": "loadbang"
												}
											},
											{
												"box": {
													"fontname": "Courier New",
													"fontsize": 9.0,
													"id": "obj-4",
													"maxclass": "newobj",
													"numinlets": 2,
													"numoutlets": 1,
													"outlettype": [
														"int"
													],
													"patching_rect": [
														77.0,
														115.0,
														27.0,
														17.0
													],
													"text": "- 1"
												}
											},
											{
												"box": {
													"comment": "value 0 - 1 (float)",
													"id": "obj-5",
													"index": 2,
													"maxclass": "outlet",
													"numinlets": 1,
													"numoutlets": 0,
													"patching_rect": [
														103.0,
														187.0,
														15.0,
														15.0
													]
												}
											},
											{
												"box": {
													"fontname": "Courier New",
													"fontsize": 9.0,
													"id": "obj-6",
													"maxclass": "comment",
													"numinlets": 1,
													"numoutlets": 0,
													"patching_rect": [
														91.0,
														206.0,
														38.0,
														17.0
													],
													"text": "value"
												}
											},
											{
												"box": {
													"comment": "index 0 -> size-1 (int)",
													"id": "obj-7",
													"index": 1,
													"maxclass": "outlet",
													"numinlets": 1,
													"numoutlets": 0,
													"patching_rect": [
														55.0,
														187.0,
														15.0,
														15.0
													]
												}
											},
											{
												"box": {
													"fontname": "Courier New",
													"fontsize": 9.0,
													"id": "obj-8",
													"maxclass": "comment",
													"numinlets": 1,
													"numoutlets": 0,
													"patching_rect": [
														41.0,
														206.0,
														38.0,
														17.0
													],
													"text": "index"
												}
											},
											{
												"box": {
													"fontname": "Courier New",
													"fontsize": 18.0,
													"id": "obj-9",
													"maxclass": "comment",
													"numinlets": 1,
													"numoutlets": 0,
													"patching_rect": [
														182.0,
														183.0,
														162.0,
														27.0
													],
													"text": "makeHanning/FO"
												}
											},
											{
												"box": {
													"fontname": "Courier New",
													"fontsize": 9.0,
													"id": "obj-10",
													"maxclass": "number",
													"numinlets": 1,
													"numoutlets": 2,
													"outlettype": [
														"",
														"bang"
													],
													"parameter_enable": 0,
													"patching_rect": [
														328.0,
														77.0,
														35.0,
														17.0
													],
													"triscale": 0.9
												}
											},
											{
												"box": {
													"comment": "size (int)",
													"id": "obj-11",
													"index": 2,
													"maxclass": "inlet",
													"numinlets": 0,
													"numoutlets": 1,
													"outlettype": [
														"int"
													],
													"patching_rect": [
														328.0,
														57.0,
														15.0,
														15.0
													]
												}
											},
											{
												"box": {
													"id": "obj-12",
													"maxclass": "button",
													"numinlets": 1,
													"numoutlets": 1,
													"outlettype": [
														"bang"
													],
													"parameter_enable": 0,
													"patching_rect": [
														37.0,
														79.0,
														15.0,
														15.0
													]
												}
											},
											{
												"box": {
													"fontname": "Courier New",
													"fontsize": 9.0,
													"id": "obj-13",
													"maxclass": "newobj",
													"numinlets": 2,
													"numoutlets": 3,
													"outlettype": [
														"bang",
														"bang",
														"int"
													],
													"patching_rect": [
														37.0,
														96.0,
														50.0,
														17.0
													],
													"text": "uzi 512"
												}
											},
											{
												"box": {
													"comment": "start bang",
													"id": "obj-14",
													"index": 1,
													"maxclass": "inlet",
													"numinlets": 0,
													"numoutlets": 1,
													"outlettype": [
														"bang"
													],
													"patching_rect": [
														37.0,
														57.0,
														15.0,
														15.0
													]
												}
											},
											{
												"box": {
													"fontname": "Courier New",
													"fontsize": 9.0,
													"id": "obj-15",
													"maxclass": "comment",
													"numinlets": 1,
													"numoutlets": 0,
													"patching_rect": [
														323.0,
														43.0,
														32.0,
														17.0
													],
													"text": "size"
												}
											},
											{
												"box": {
													"fontname": "Courier New",
													"fontsize": 9.0,
													"id": "obj-16",
													"maxclass": "comment",
													"numinlets": 1,
													"numoutlets": 0,
													"patching_rect": [
														32.0,
														43.0,
														38.0,
														17.0
													],
													"text": "start"
												}
											},
											{
												"box": {
													"fontname": "Courier New",
													"fontsize": 9.0,
													"id": "obj-17",
													"maxclass": "comment",
													"numinlets": 1,
													"numoutlets": 0,
													"patching_rect": [
														182.0,
														208.0,
														104.0,
														17.0
													],
													"text": "opt. arg. = size"
												}
											}
										],
										"lines": [
											{
												"patchline": {
													"destination": [
														"obj-10",
														0
													],
													"midpoints": [
														248.5,
														74.0,
														337.5,
														74.0
													],
													"source": [
														"obj-1",
														0
													]
												}
											},
											{
												"patchline": {
													"destination": [
														"obj-13",
														1
													],
													"midpoints": [
														337.5,
														93.0,
														77.5,
														93.0
													],
													"order": 1,
													"source": [
														"obj-10",
														0
													]
												}
											},
											{
												"patchline": {
													"destination": [
														"obj-2",
														1
													],
													"order": 0,
													"source": [
														"obj-10",
														0
													]
												}
											},
											{
												"patchline": {
													"destination": [
														"obj-10",
														0
													],
													"source": [
														"obj-11",
														0
													]
												}
											},
											{
												"patchline": {
													"destination": [
														"obj-13",
														0
													],
													"source": [
														"obj-12",
														0
													]
												}
											},
											{
												"patchline": {
													"destination": [
														"obj-4",
														0
													],
													"source": [
														"obj-13",
														2
													]
												}
											},
											{
												"patchline": {
													"destination": [
														"obj-12",
														0
													],
													"source": [
														"obj-14",
														0
													]
												}
											},
											{
												"patchline": {
													"destination": [
														"obj-5",
														0
													],
													"source": [
														"obj-2",
														0
													]
												}
											},
											{
												"patchline": {
													"destination": [
														"obj-1",
														0
													],
													"source": [
														"obj-3",
														0
													]
												}
											},
											{
												"patchline": {
													"destination": [
														"obj-2",
														0
													],
													"midpoints": [
														86.5,
														134.0,
														112.5,
														134.0
													],
													"order": 0,
													"source": [
														"obj-4",
														0
													]
												}
											},
											{
												"patchline": {
													"destination": [
														"obj-7",
														0
													],
													"order": 1,
													"source": [
														"obj-4",
														0
													]
												}
											}
										]
									},
									"patching_rect": [
										38.0,
										112.0,
										87.0,
										20.0
									],
									"saved_object_attributes": {
										"description": "",
										"digest": "",
										"globalpatchername": "",
										"tags": ""
									},
									"text": "p hanning 512"
								}
							},
							{
								"box": {
									"fontname": "Arial",
									"fontsize": 12.0,
									"id": "obj-29",
									"maxclass": "message",
									"numinlets": 2,
									"numoutlets": 1,
									"outlettype": [
										""
									],
									"patching_rect": [
										173.0,
										125.0,
										93.0,
										18.0
									],
									"text": "sizeinsamps $1"
								}
							},
							{
								"box": {
									"fontname": "Arial",
									"fontsize": 12.0,
									"id": "obj-3",
									"maxclass": "newobj",
									"numinlets": 1,
									"numoutlets": 2,
									"outlettype": [
										"float",
										"bang"
									],
									"patching_rect": [
										173.0,
										155.0,
										71.0,
										20.0
									],
									"text": "buffer~ env"
								}
							},
							{
								"box": {
									"fontname": "Geneva",
									"fontsize": 9.0,
									"id": "obj-30",
									"maxclass": "newobj",
									"numinlets": 3,
									"numoutlets": 1,
									"outlettype": [
										"float"
									],
									"patching_rect": [
										38.0,
										169.0,
										56.0,
										18.0
									],
									"text": "peek~ env"
								}
							},
							{
								"box": {
									"fontname": "Geneva",
									"fontsize": 9.0,
									"id": "obj-36",
									"maxclass": "newobj",
									"numinlets": 2,
									"numoutlets": 1,
									"outlettype": [
										""
									],
									"patching_rect": [
										38.0,
										143.0,
										87.0,
										18.0
									],
									"text": "pack 0 0."
								}
							}
						],
						"lines": [
							{
								"patchline": {
									"destination": [
										"obj-9",
										0
									],
									"source": [
										"obj-1",
										0
									]
								}
							},
							{
								"patchline": {
									"destination": [
										"obj-5",
										0
									],
									"source": [
										"obj-10",
										0
									]
								}
							},
							{
								"patchline": {
									"destination": [
										"obj-6",
										0
									],
									"source": [
										"obj-2",
										0
									]
								}
							},
							{
								"patchline": {
									"destination": [
										"obj-3",
										0
									],
									"midpoints": [
										182.5,
										145.0,
										182.5,
										145.0
									],
									"source": [
										"obj-29",
										0
									]
								}
							},
							{
								"patchline": {
									"destination": [
										"obj-30",
										0
									],
									"midpoints": [
										47.5,
										161.0,
										47.5,
										161.0
									],
									"source": [
										"obj-36",
										0
									]
								}
							},
							{
								"patchline": {
									"destination": [
										"obj-36",
										1
									],
									"midpoints": [
										115.5,
										134.0,
										115.5,
										134.0
									],
									"source": [
										"obj-4",
										1
									]
								}
							},
							{
								"patchline": {
									"destination": [
										"obj-36",
										0
									],
									"midpoints": [
										47.5,
										134.0,
										47.5,
										134.0
									],
									"source": [
										"obj-4",
										0
									]
								}
							},
							{
								"patchline": {
									"destination": [
										"obj-6",
										0
									],
									"source": [
										"obj-5",
										0
									]
								}
							},
							{
								"patchline": {
									"destination": [
										"obj-29",
										0
									],
									"source": [
										"obj-6",
										2
									]
								}
							},
							{
								"patchline": {
									"destination": [
										"obj-4",
										1
									],
									"source": [
										"obj-6",
										1
									]
								}
							},
							{
								"patchline": {
									"destination": [
										"obj-4",
										0
									],
									"source": [
										"obj-6",
										0
									]
								}
							},
							{
								"patchline": {
									"destination": [
										"obj-8",
										0
									],
									"midpoints": [
										455.0,
										159.0
									],
									"source": [
										"obj-7",
										0
									]
								}
							},
							{
								"patchline": {
									"destination": [
										"obj-1",
										0
									],
									"source": [
										"obj-8",
										0
									]
								}
							},
							{
								"patchline": {
									"destination": [
										"obj-2",
										0
									],
									"source": [
										"obj-9",
										0
									]
								}
							}
						]
					},
					"patching_rect": [
						15.0,
						668.0,
						53.0,
						22.0
					],
					"saved_object_attributes": {
						"description": "",
						"digest": "",
						"globalpatchername": "",
						"tags": ""
					},
					"text": "p env"
				}
			},
			{
				"box": {
					"bgcolor": [
						1.0,
						0.0,
						0.165438,
						1.0
					],
					"fontname": "Arial",
					"fontsize": 12.0,
					"id": "obj-110",
					"maxclass": "newobj",
					"numinlets": 1,
					"numoutlets": 0,
					"patcher": {
						"fileversion": 1,
						"appversion": {
							"major": 8,
							"minor": 2,
							"revision": 2,
							"architecture": "x64",
							"modernui": 1
						},
						"classnamespace": "box",
						"rect": [
							60.0,
							120.0,
							1290.0,
							300.0
						],
						"bglocked": 0,
						"openinpresentation": 0,
						"default_fontsize": 12.0,
						"default_fontface": 0,
						"default_fontname": "Arial",
						"gridonopen": 1,
						"gridsize": [
							15.0,
							15.0
						],
						"gridsnaponopen": 1,
						"objectsnaponopen": 1,
						"statusbarvisible": 2,
						"toolbarvisible": 1,
						"lefttoolbarpinned": 0,
						"toptoolbarpinned": 0,
						"righttoolbarpinned": 0,
						"bottomtoolbarpinned": 0,
						"toolbars_unpinned_last_save": 0,
						"tallnewobj": 0,
						"boxanimatetime": 200,
						"enablehscroll": 1,
						"enablevscroll": 1,
						"devicewidth": 0.0,
						"description": "",
						"digest": "",
						"tags": "",
						"style": "",
						"subpatcher_template": "",
						"assistshowspatchername": 0,
						"visible": 1,
						"boxes": [
							{
								"box": {
									"fontname": "Arial",
									"fontsize": 11.0,
									"id": "obj-7",
									"linecount": 2,
									"maxclass": "comment",
									"numinlets": 1,
									"numoutlets": 0,
									"patching_rect": [
										640.0,
										75.0,
										119.0,
										32.0
									],
									"text": "setPhase\nread point, 0-1"
								}
							},
							{
								"box": {
									"fontname": "Arial",
									"fontsize": 12.0,
									"format": 6,
									"id": "obj-6",
									"maxclass": "flonum",
									"maximum": 1.0,
									"minimum": 0.0,
									"numinlets": 1,
									"numoutlets": 2,
									"outlettype": [
										"",
										"bang"
									],
									"parameter_enable": 0,
									"patching_rect": [
										640.0,
										143.0,
										50.0,
										22.0
									]
								}
							},
							{
								"box": {
									"fontname": "Arial",
									"fontsize": 12.0,
									"id": "obj-14",
									"maxclass": "message",
									"numinlets": 2,
									"numoutlets": 1,
									"outlettype": [
										""
									],
									"patching_rect": [
										640.0,
										173.0,
										77.0,
										22.0
									],
									"text": "setPhase $1"
								}
							},
							{
								"box": {
									"comment": "",
									"id": "obj-10",
									"index": 1,
									"maxclass": "inlet",
									"numinlets": 0,
									"numoutlets": 1,
									"outlettype": [
										""
									],
									"patching_rect": [
										15.0,
										8.0,
										25.0,
										25.0
									]
								}
							},
							{
								"box": {
									"id": "obj-9",
									"maxclass": "button",
									"numinlets": 1,
									"numoutlets": 1,
									"outlettype": [
										"bang"
									],
									"parameter_enable": 0,
									"patching_rect": [
										15.0,
										40.0,
										20.0,
										20.0
									]
								}
							},
							{
								"box": {
									"fontname": "Arial",
									"fontsize": 11.0,
									"id": "obj-5",
									"linecount": 2,
									"maxclass": "comment",
									"numinlets": 1,
									"numoutlets": 0,
									"patching_rect": [
										1015.0,
										75.0,
										119.0,
										32.0
									],
									"text": "setWidth\nstereo width, 0-1"
								}
							},
							{
								"box": {
									"fontname": "Arial",
									"fontsize": 11.0,
									"id": "obj-92",
									"linecount": 3,
									"maxclass": "comment",
									"numinlets": 1,
									"numoutlets": 0,
									"patching_rect": [
										1140.0,
										75.0,
										119.0,
										46.0
									],
									"text": "setPanningMode\n0 = random position\n1 = setWidth is the position"
								}
							},
							{
								"box": {
									"id": "obj-1",
									"maxclass": "toggle",
									"numinlets": 1,
									"numoutlets": 1,
									"outlettype": [
										"int"
									],
									"parameter_enable": 0,
									"patching_rect": [
										1140.0,
										143.0,
										20.0,
										20.0
									]
								}
							},
							{
								"box": {
									"fontname": "Arial",
									"fontsize": 12.0,
									"id": "obj-2",
									"maxclass": "message",
									"numinlets": 2,
									"numoutlets": 1,
									"outlettype": [
										""
									],
									"patching_rect": [
										1140.0,
										173.0,
										117.0,
										22.0
									],
									"text": "setPanningMode $1"
								}
							},
							{
								"box": {
									"fontname": "Arial",
									"fontsize": 12.0,
									"format": 6,
									"id": "obj-3",
									"maxclass": "flonum",
									"maximum": 1.0,
									"minimum": 0.0,
									"numinlets": 1,
									"numoutlets": 2,
									"outlettype": [
										"",
										"bang"
									],
									"parameter_enable": 0,
									"patching_rect": [
										1015.0,
										143.0,
										50.0,
										22.0
									]
								}
							},
							{
								"box": {
									"fontname": "Arial",
									"fontsize": 12.0,
									"id": "obj-95",
									"maxclass": "message",
									"numinlets": 2,
									"numoutlets": 1,
									"outlettype": [
										""
									],
									"patching_rect": [
										1015.0,
										173.0,
										74.0,
										22.0
									],
									"text": "setWidth $1"
								}
							},
							{
								"box": {
									"fontname": "Arial",
									"fontsize": 12.0,
									"id": "obj-4",
									"maxclass": "newobj",
									"numinlets": 1,
									"numoutlets": 0,
									"patching_rect": [
										1015.0,
										225.0,
										88.0,
										22.0
									],
									"text": "send ---engine"
								}
							},
							{
								"box": {
									"fontname": "Arial",
									"fontsize": 12.0,
									"id": "obj-80",
									"maxclass": "message",
									"numinlets": 2,
									"numoutlets": 1,
									"outlettype": [
										""
									],
									"patching_rect": [
										515.0,
										173.0,
										72.0,
										22.0
									],
									"text": "setLevel $1"
								}
							},
							{
								"box": {
									"fontname": "Arial",
									"fontsize": 12.0,
									"id": "obj-64",
									"maxclass": "newobj",
									"numinlets": 1,
									"numoutlets": 1,
									"outlettype": [
										""
									],
									"patching_rect": [
										515.0,
										115.0,
										82.0,
										22.0
									],
									"text": "loadmess 0.5"
								}
							},
							{
								"box": {
									"fontname": "Arial",
									"fontsize": 11.0,
									"id": "obj-66",
									"maxclass": "comment",
									"numinlets": 1,
									"numoutlets": 0,
									"patching_rect": [
										515.0,
										75.0,
										119.0,
										32.0
									],
									"text": "setLevel\namplitude, 0-1",
									"linecount": 2
								}
							},
							{
								"box": {
									"fontname": "Arial",
									"fontsize": 12.0,
									"format": 6,
									"id": "obj-68",
									"maxclass": "flonum",
									"maximum": 1.0,
									"minimum": 0.0,
									"numinlets": 1,
									"numoutlets": 2,
									"outlettype": [
										"",
										"bang"
									],
									"parameter_enable": 0,
									"patching_rect": [
										515.0,
										143.0,
										50.0,
										22.0
									]
								}
							},
							{
								"box": {
									"fontname": "Arial",
									"fontsize": 12.0,
									"id": "obj-76",
									"maxclass": "newobj",
									"numinlets": 1,
									"numoutlets": 1,
									"outlettype": [
										""
									],
									"patching_rect": [
										765.0,
										115.0,
										72.0,
										22.0
									],
									"text": "loadmess 0"
								}
							},
							{
								"box": {
									"fontname": "Arial",
									"fontsize": 11.0,
									"id": "obj-75",
									"linecount": 2,
									"maxclass": "comment",
									"numinlets": 1,
									"numoutlets": 0,
									"patching_rect": [
										890.0,
										75.0,
										119.0,
										32.0
									],
									"text": "setRndPitch\nrandom, semitones"
								}
							},
							{
								"box": {
									"fontname": "Arial",
									"fontsize": 11.0,
									"id": "obj-74",
									"linecount": 2,
									"maxclass": "comment",
									"numinlets": 1,
									"numoutlets": 0,
									"patching_rect": [
										765.0,
										75.0,
										119.0,
										32.0
									],
									"text": "setPitch\nsemitones"
								}
							},
							{
								"box": {
									"fontname": "Arial",
									"fontsize": 12.0,
									"id": "obj-73",
									"maxclass": "newobj",
									"numinlets": 1,
									"numoutlets": 0,
									"patching_rect": [
										765.0,
										225.0,
										88.0,
										22.0
									],
									"text": "send ---engine"
								}
							},
							{
								"box": {
									"fontname": "Arial",
									"fontsize": 11.0,
									"id": "obj-72",
									"linecount": 2,
									"maxclass": "comment",
									"numinlets": 1,
									"numoutlets": 0,
									"patching_rect": [
										140.0,
										75.0,
										119.0,
										32.0
									],
									"text": "setRndDensity\nrandom part, 0-1"
								}
							},
							{
								"box": {
									"fontname": "Arial",
									"fontsize": 11.0,
									"id": "obj-70",
									"maxclass": "comment",
									"numinlets": 1,
									"numoutlets": 0,
									"patching_rect": [
										15.0,
										75.0,
										119.0,
										32.0
									],
									"text": "setDensity\ngrains per second",
									"linecount": 2
								}
							},
							{
								"box": {
									"fontname": "Arial",
									"fontsize": 12.0,
									"id": "obj-71",
									"maxclass": "newobj",
									"numinlets": 1,
									"numoutlets": 1,
									"outlettype": [
										""
									],
									"patching_rect": [
										15.0,
										115.0,
										79.0,
										22.0
									],
									"text": "loadmess 10"
								}
							},
							{
								"box": {
									"fontname": "Arial",
									"fontsize": 12.0,
									"id": "obj-69",
									"maxclass": "newobj",
									"numinlets": 1,
									"numoutlets": 0,
									"patching_rect": [
										265.0,
										225.0,
										88.0,
										22.0
									],
									"text": "send ---engine"
								}
							},
							{
								"box": {
									"fontname": "Arial",
									"fontsize": 11.0,
									"id": "obj-67",
									"linecount": 2,
									"maxclass": "comment",
									"numinlets": 1,
									"numoutlets": 0,
									"patching_rect": [
										390.0,
										75.0,
										119.0,
										32.0
									],
									"text": "setRndLength\nrandom part, 0-1"
								}
							},
							{
								"box": {
									"fontname": "Arial",
									"fontsize": 11.0,
									"id": "obj-62",
									"maxclass": "comment",
									"numinlets": 1,
									"numoutlets": 0,
									"patching_rect": [
										265.0,
										75.0,
										119.0,
										32.0
									],
									"text": "setLength\ngrain length, ms",
									"linecount": 2
								}
							},
							{
								"box": {
									"fontname": "Arial",
									"fontsize": 12.0,
									"id": "obj-63",
									"maxclass": "newobj",
									"numinlets": 1,
									"numoutlets": 1,
									"outlettype": [
										""
									],
									"patching_rect": [
										265.0,
										115.0,
										79.0,
										22.0
									],
									"text": "loadmess 85"
								}
							},
							{
								"box": {
									"fontname": "Arial",
									"fontsize": 12.0,
									"format": 6,
									"id": "obj-30",
									"maxclass": "flonum",
									"maximum": 12.0,
									"minimum": 0.0,
									"numinlets": 1,
									"numoutlets": 2,
									"outlettype": [
										"",
										"bang"
									],
									"parameter_enable": 0,
									"patching_rect": [
										890.0,
										143.0,
										50.0,
										22.0
									]
								}
							},
							{
								"box": {
									"fontname": "Arial",
									"fontsize": 12.0,
									"id": "obj-37",
									"maxclass": "message",
									"numinlets": 2,
									"numoutlets": 1,
									"outlettype": [
										""
									],
									"patching_rect": [
										890.0,
										173.0,
										92.0,
										22.0
									],
									"text": "setRndPitch $1"
								}
							},
							{
								"box": {
									"fontname": "Arial",
									"fontsize": 12.0,
									"format": 6,
									"id": "obj-51",
									"maxclass": "flonum",
									"maximum": 48.0,
									"minimum": -48.0,
									"numinlets": 1,
									"numoutlets": 2,
									"outlettype": [
										"",
										"bang"
									],
									"parameter_enable": 0,
									"patching_rect": [
										765.0,
										143.0,
										50.0,
										22.0
									]
								}
							},
							{
								"box": {
									"fontname": "Arial",
									"fontsize": 12.0,
									"id": "obj-61",
									"maxclass": "message",
									"numinlets": 2,
									"numoutlets": 1,
									"outlettype": [
										""
									],
									"patching_rect": [
										765.0,
										173.0,
										70.0,
										22.0
									],
									"text": "setPitch $1"
								}
							},
							{
								"box": {
									"fontname": "Arial",
									"fontsize": 12.0,
									"format": 6,
									"id": "obj-57",
									"maxclass": "flonum",
									"maximum": 1.0,
									"minimum": 0.0,
									"numinlets": 1,
									"numoutlets": 2,
									"outlettype": [
										"",
										"bang"
									],
									"parameter_enable": 0,
									"patching_rect": [
										390.0,
										143.0,
										50.0,
										22.0
									]
								}
							},
							{
								"box": {
									"fontname": "Arial",
									"fontsize": 12.0,
									"id": "obj-58",
									"maxclass": "message",
									"numinlets": 2,
									"numoutlets": 1,
									"outlettype": [
										""
									],
									"patching_rect": [
										390.0,
										173.0,
										102.0,
										22.0
									],
									"text": "setRndLength $1"
								}
							},
							{
								"box": {
									"fontname": "Arial",
									"fontsize": 12.0,
									"format": 6,
									"id": "obj-59",
									"maxclass": "flonum",
									"maximum": 2000.0,
									"minimum": 0.0,
									"numinlets": 1,
									"numoutlets": 2,
									"outlettype": [
										"",
										"bang"
									],
									"parameter_enable": 0,
									"patching_rect": [
										265.0,
										143.0,
										50.0,
										22.0
									]
								}
							},
							{
								"box": {
									"fontname": "Arial",
									"fontsize": 12.0,
									"id": "obj-60",
									"maxclass": "message",
									"numinlets": 2,
									"numoutlets": 1,
									"outlettype": [
										""
									],
									"patching_rect": [
										265.0,
										173.0,
										80.0,
										22.0
									],
									"text": "setLength $1"
								}
							},
							{
								"box": {
									"fontname": "Arial",
									"fontsize": 12.0,
									"format": 6,
									"id": "obj-56",
									"maxclass": "flonum",
									"maximum": 1.0,
									"minimum": 0.0,
									"numinlets": 1,
									"numoutlets": 2,
									"outlettype": [
										"",
										"bang"
									],
									"parameter_enable": 0,
									"patching_rect": [
										140.0,
										143.0,
										50.0,
										22.0
									]
								}
							},
							{
								"box": {
									"fontname": "Arial",
									"fontsize": 12.0,
									"id": "obj-54",
									"maxclass": "message",
									"numinlets": 2,
									"numoutlets": 1,
									"outlettype": [
										""
									],
									"patching_rect": [
										140.0,
										173.0,
										105.0,
										22.0
									],
									"text": "setRndDensity $1"
								}
							},
							{
								"box": {
									"fontname": "Arial",
									"fontsize": 12.0,
									"format": 6,
									"id": "obj-39",
									"maxclass": "flonum",
									"maximum": 2000.0,
									"minimum": 0.0,
									"numinlets": 1,
									"numoutlets": 2,
									"outlettype": [
										"",
										"bang"
									],
									"parameter_enable": 0,
									"patching_rect": [
										15.0,
										143.0,
										50.0,
										22.0
									]
								}
							},
							{
								"box": {
									"fontname": "Arial",
									"fontsize": 12.0,
									"id": "obj-38",
									"maxclass": "message",
									"numinlets": 2,
									"numoutlets": 1,
									"outlettype": [
										""
									],
									"patching_rect": [
										15.0,
										173.0,
										83.0,
										22.0
									],
									"text": "setDensity $1"
								}
							},
							{
								"box": {
									"id": "obj-901",
									"maxclass": "comment",
									"numinlets": 1,
									"numoutlets": 0,
									"fontname": "Arial",
									"text": "Parameters of ape.density~  —  every message goes to the object through  send ---engine  /  receive ---engine",
									"fontsize": 13.0,
									"patching_rect": [
										50.0,
										10.0,
										900.0,
										21.0
									],
									"fontface": 1
								}
							},
							{
								"box": {
									"id": "obj-902",
									"maxclass": "comment",
									"numinlets": 1,
									"numoutlets": 0,
									"fontname": "Arial",
									"text": "a bang in the inlet = back to the starting values",
									"fontsize": 11.0,
									"patching_rect": [
										42.0,
										42.0,
										400.0,
										19.0
									]
								}
							}
						],
						"lines": [
							{
								"patchline": {
									"destination": [
										"obj-2",
										0
									],
									"source": [
										"obj-1",
										0
									]
								}
							},
							{
								"patchline": {
									"destination": [
										"obj-9",
										0
									],
									"source": [
										"obj-10",
										0
									]
								}
							},
							{
								"patchline": {
									"destination": [
										"obj-69",
										0
									],
									"source": [
										"obj-14",
										0
									]
								}
							},
							{
								"patchline": {
									"destination": [
										"obj-4",
										0
									],
									"source": [
										"obj-2",
										0
									]
								}
							},
							{
								"patchline": {
									"destination": [
										"obj-95",
										0
									],
									"source": [
										"obj-3",
										0
									]
								}
							},
							{
								"patchline": {
									"destination": [
										"obj-37",
										0
									],
									"source": [
										"obj-30",
										0
									]
								}
							},
							{
								"patchline": {
									"destination": [
										"obj-73",
										0
									],
									"source": [
										"obj-37",
										0
									]
								}
							},
							{
								"patchline": {
									"destination": [
										"obj-69",
										0
									],
									"source": [
										"obj-38",
										0
									]
								}
							},
							{
								"patchline": {
									"destination": [
										"obj-38",
										0
									],
									"source": [
										"obj-39",
										0
									]
								}
							},
							{
								"patchline": {
									"destination": [
										"obj-61",
										0
									],
									"source": [
										"obj-51",
										0
									]
								}
							},
							{
								"patchline": {
									"destination": [
										"obj-69",
										0
									],
									"source": [
										"obj-54",
										0
									]
								}
							},
							{
								"patchline": {
									"destination": [
										"obj-54",
										0
									],
									"source": [
										"obj-56",
										0
									]
								}
							},
							{
								"patchline": {
									"destination": [
										"obj-58",
										0
									],
									"source": [
										"obj-57",
										0
									]
								}
							},
							{
								"patchline": {
									"destination": [
										"obj-69",
										0
									],
									"source": [
										"obj-58",
										0
									]
								}
							},
							{
								"patchline": {
									"destination": [
										"obj-60",
										0
									],
									"source": [
										"obj-59",
										0
									]
								}
							},
							{
								"patchline": {
									"destination": [
										"obj-14",
										0
									],
									"source": [
										"obj-6",
										0
									]
								}
							},
							{
								"patchline": {
									"destination": [
										"obj-69",
										0
									],
									"source": [
										"obj-60",
										0
									]
								}
							},
							{
								"patchline": {
									"destination": [
										"obj-73",
										0
									],
									"source": [
										"obj-61",
										0
									]
								}
							},
							{
								"patchline": {
									"destination": [
										"obj-59",
										0
									],
									"source": [
										"obj-63",
										0
									]
								}
							},
							{
								"patchline": {
									"destination": [
										"obj-68",
										0
									],
									"source": [
										"obj-64",
										0
									]
								}
							},
							{
								"patchline": {
									"destination": [
										"obj-80",
										0
									],
									"source": [
										"obj-68",
										0
									]
								}
							},
							{
								"patchline": {
									"destination": [
										"obj-39",
										0
									],
									"source": [
										"obj-71",
										0
									]
								}
							},
							{
								"patchline": {
									"destination": [
										"obj-51",
										0
									],
									"source": [
										"obj-76",
										0
									]
								}
							},
							{
								"patchline": {
									"destination": [
										"obj-69",
										0
									],
									"source": [
										"obj-80",
										0
									]
								}
							},
							{
								"patchline": {
									"destination": [
										"obj-63",
										0
									],
									"order": 2,
									"source": [
										"obj-9",
										0
									]
								}
							},
							{
								"patchline": {
									"destination": [
										"obj-64",
										0
									],
									"order": 1,
									"source": [
										"obj-9",
										0
									]
								}
							},
							{
								"patchline": {
									"destination": [
										"obj-71",
										0
									],
									"order": 3,
									"source": [
										"obj-9",
										0
									]
								}
							},
							{
								"patchline": {
									"destination": [
										"obj-76",
										0
									],
									"order": 0,
									"source": [
										"obj-9",
										0
									]
								}
							},
							{
								"patchline": {
									"destination": [
										"obj-4",
										0
									],
									"source": [
										"obj-95",
										0
									]
								}
							}
						]
					},
					"patching_rect": [
						655.0,
						302.0,
						119.0,
						22.0
					],
					"saved_object_attributes": {
						"description": "",
						"digest": "",
						"globalpatchername": "",
						"tags": ""
					},
					"text": "p single_parameters",
					"textcolor": [
						0.0,
						0.0,
						0.0,
						1.0
					]
				}
			},
			{
				"box": {
					"fontname": "Arial",
					"fontsize": 12.0,
					"id": "obj-101",
					"maxclass": "message",
					"numinlets": 2,
					"numoutlets": 1,
					"outlettype": [
						""
					],
					"patching_rect": [
						540.0,
						580.0,
						49.0,
						22.0
					],
					"text": "set env"
				}
			},
			{
				"box": {
					"fontname": "Arial",
					"fontsize": 12.0,
					"id": "obj-100",
					"maxclass": "message",
					"numinlets": 2,
					"numoutlets": 1,
					"outlettype": [
						""
					],
					"patching_rect": [
						115.0,
						302.0,
						93.0,
						22.0
					],
					"text": "setSample smp"
				}
			},
			{
				"box": {
					"allowdrag": 0,
					"buffername": "smp",
					"id": "obj-96",
					"maxclass": "waveform~",
					"numinlets": 5,
					"numoutlets": 6,
					"outlettype": [
						"float",
						"float",
						"float",
						"float",
						"list",
						""
					],
					"patching_rect": [
						480.0,
						608.0,
						300.0,
						100.0
					],
					"setmode": 4
				}
			},
			{
				"box": {
					"fontname": "Arial",
					"fontsize": 12.0,
					"id": "obj-65",
					"maxclass": "newobj",
					"numinlets": 0,
					"numoutlets": 1,
					"outlettype": [
						""
					],
					"patching_rect": [
						540.0,
						302.0,
						101.0,
						22.0
					],
					"text": "receive ---engine"
				}
			},
			{
				"box": {
					"fontname": "Arial",
					"fontsize": 12.0,
					"id": "obj-8",
					"maxclass": "message",
					"numinlets": 2,
					"numoutlets": 1,
					"outlettype": [
						""
					],
					"patching_rect": [
						215.0,
						302.0,
						108.0,
						22.0
					],
					"text": "setWindowing env"
				}
			},
			{
				"box": {
					"fontname": "Arial",
					"fontsize": 11.0,
					"id": "obj-36",
					"maxclass": "comment",
					"numinlets": 1,
					"numoutlets": 0,
					"patching_rect": [
						15.0,
						282.0,
						340.0,
						19.0
					],
					"text": "choose the buffers: sound, window"
				}
			},
			{
				"box": {
					"fontname": "Arial",
					"fontsize": 11.0,
					"id": "obj-33",
					"maxclass": "comment",
					"numinlets": 1,
					"numoutlets": 0,
					"patching_rect": [
						805.0,
						150.0,
						89.0,
						46.0
					],
					"text": "info\nasks how many\ngrains are active",
					"linecount": 3
				}
			},
			{
				"box": {
					"fontname": "Arial",
					"fontsize": 12.0,
					"id": "obj-28",
					"maxclass": "message",
					"numinlets": 2,
					"numoutlets": 1,
					"outlettype": [
						""
					],
					"patching_rect": [
						540.0,
						347.0,
						50.0,
						22.0
					],
					"text": "510"
				}
			},
			{
				"box": {
					"fontname": "Arial",
					"fontsize": 12.0,
					"id": "obj-52",
					"maxclass": "message",
					"numinlets": 2,
					"numoutlets": 1,
					"outlettype": [
						""
					],
					"patching_rect": [
						15.0,
						302.0,
						92.0,
						22.0
					],
					"text": "setbuf smp env"
				}
			},
			{
				"box": {
					"fontname": "Arial",
					"fontsize": 11.0,
					"id": "obj-48",
					"maxclass": "comment",
					"numinlets": 1,
					"numoutlets": 0,
					"patching_rect": [
						166.0,
						608.0,
						220.0,
						19.0
					],
					"text": "load another sound file"
				}
			},
			{
				"box": {
					"fontname": "Arial",
					"fontsize": 12.0,
					"id": "obj-49",
					"maxclass": "message",
					"numinlets": 2,
					"numoutlets": 1,
					"outlettype": [
						""
					],
					"patching_rect": [
						15.0,
						606.0,
						84.0,
						22.0
					],
					"text": "read jongly.aif"
				}
			},
			{
				"box": {
					"fontname": "Arial",
					"fontsize": 12.0,
					"id": "obj-50",
					"maxclass": "newobj",
					"numinlets": 1,
					"numoutlets": 2,
					"outlettype": [
						"float",
						"bang"
					],
					"patching_rect": [
						15.0,
						636.0,
						157.0,
						22.0
					],
					"text": "buffer~ smp drumloop.aif -1"
				}
			},
			{
				"box": {
					"bufsize": 256,
					"calccount": 4,
					"id": "obj-25",
					"maxclass": "scope~",
					"numinlets": 2,
					"numoutlets": 0,
					"patching_rect": [
						480.0,
						387.0,
						300.0,
						150.0
					]
				}
			},
			{
				"box": {
					"fontname": "Arial",
					"fontsize": 11.0,
					"id": "obj-35",
					"maxclass": "comment",
					"numinlets": 1,
					"numoutlets": 0,
					"patching_rect": [
						15.0,
						96.0,
						560.0,
						19.0
					],
					"text": "Alessandro Petrolati  ·  ape.density~  ·  © 2026  ·  www.apesoft.it"
				}
			},
			{
				"box": {
					"id": "obj-9",
					"lastchannelcount": 0,
					"maxclass": "live.gain~",
					"numinlets": 2,
					"numoutlets": 5,
					"outlettype": [
						"signal",
						"signal",
						"",
						"float",
						"list"
					],
					"parameter_enable": 1,
					"patching_rect": [
						385.0,
						387.0,
						73.0,
						120.0
					],
					"saved_attribute_attributes": {
						"valueof": {
							"parameter_initial": [
								0.0
							],
							"parameter_longname": "live.gain~",
							"parameter_mmax": 6.0,
							"parameter_mmin": -70.0,
							"parameter_shortname": "live.gain~",
							"parameter_type": 0,
							"parameter_unitstyle": 4
						}
					},
					"varname": "live.gain~"
				}
			},
			{
				"box": {
					"fontname": "Arial",
					"fontsize": 11.0,
					"id": "obj-20",
					"maxclass": "comment",
					"numinlets": 1,
					"numoutlets": 0,
					"patching_rect": [
						725.0,
						150.0,
						74.0,
						32.0
					],
					"text": "grain\none grain now",
					"linecount": 2
				}
			},
			{
				"box": {
					"fontname": "Arial",
					"fontsize": 11.0,
					"id": "obj-15",
					"maxclass": "comment",
					"numinlets": 1,
					"numoutlets": 0,
					"patching_rect": [
						15.0,
						150.0,
						84.0,
						32.0
					],
					"text": "start\n1 = on, 0 = off",
					"linecount": 2
				}
			},
			{
				"box": {
					"bgcolor": [
						1.0,
						0.812972,
						0.047021,
						1.0
					],
					"checkedcolor": [
						0.0,
						0.0,
						0.0,
						1.0
					],
					"id": "obj-13",
					"maxclass": "toggle",
					"numinlets": 1,
					"numoutlets": 1,
					"outlettype": [
						"int"
					],
					"parameter_enable": 0,
					"patching_rect": [
						15.0,
						218.0,
						20.0,
						20.0
					],
					"uncheckedcolor": [
						0.0,
						0.0,
						0.0,
						0.0
					]
				}
			},
			{
				"box": {
					"id": "obj-10",
					"maxclass": "toggle",
					"numinlets": 1,
					"numoutlets": 1,
					"outlettype": [
						"int"
					],
					"parameter_enable": 0,
					"patching_rect": [
						105.0,
						218.0,
						20.0,
						20.0
					]
				}
			},
			{
				"box": {
					"fontname": "Arial",
					"fontsize": 12.0,
					"id": "obj-5",
					"maxclass": "message",
					"numinlets": 2,
					"numoutlets": 1,
					"outlettype": [
						""
					],
					"patching_rect": [
						725.0,
						248.0,
						37.0,
						22.0
					],
					"text": "grain"
				}
			},
			{
				"box": {
					"id": "obj-1",
					"maxclass": "ezdac~",
					"numinlets": 2,
					"numoutlets": 0,
					"patching_rect": [
						385.0,
						522.0,
						33.0,
						33.0
					]
				}
			},
			{
				"box": {
					"fontname": "Arial",
					"fontsize": 12.0,
					"id": "obj-19",
					"maxclass": "newobj",
					"numinlets": 1,
					"numoutlets": 3,
					"outlettype": [
						"signal",
						"signal",
						"int"
					],
					"patching_rect": [
						385.0,
						347.0,
						126.0,
						22.0
					],
					"text": "ape.density~ smp env"
				}
			},
			{
				"box": {
					"fontname": "Arial",
					"fontsize": 12.0,
					"id": "obj-3",
					"maxclass": "message",
					"numinlets": 2,
					"numoutlets": 1,
					"outlettype": [
						""
					],
					"patching_rect": [
						805.0,
						248.0,
						50.0,
						22.0
					],
					"text": "info"
				}
			},
			{
				"box": {
					"fontname": "Arial",
					"fontsize": 12.0,
					"id": "obj-7",
					"maxclass": "message",
					"numinlets": 2,
					"numoutlets": 1,
					"outlettype": [
						""
					],
					"patching_rect": [
						105.0,
						248.0,
						56.0,
						22.0
					],
					"text": "interp $1"
				}
			},
			{
				"box": {
					"fontname": "Arial",
					"fontsize": 12.0,
					"id": "obj-11",
					"maxclass": "message",
					"numinlets": 2,
					"numoutlets": 1,
					"outlettype": [
						""
					],
					"patching_rect": [
						15.0,
						248.0,
						50.0,
						22.0
					],
					"text": "start $1"
				}
			},
			{
				"box": {
					"fontname": "Arial",
					"fontsize": 12.0,
					"id": "obj-99",
					"maxclass": "message",
					"numinlets": 2,
					"numoutlets": 1,
					"outlettype": [
						""
					],
					"patching_rect": [
						480.0,
						580.0,
						53.0,
						22.0
					],
					"text": "set smp"
				}
			},
			{
				"box": {
					"fontname": "Arial",
					"fontsize": 12.0,
					"id": "obj-12",
					"maxclass": "message",
					"numinlets": 2,
					"numoutlets": 1,
					"outlettype": [
						""
					],
					"patching_rect": [
						295.0,
						248.0,
						66.0,
						22.0
					],
					"text": "reverse $1"
				}
			},
			{
				"box": {
					"fontname": "Arial",
					"fontsize": 12.0,
					"id": "obj-18",
					"maxclass": "message",
					"numinlets": 2,
					"numoutlets": 1,
					"outlettype": [
						""
					],
					"patching_rect": [
						195.0,
						248.0,
						67.0,
						22.0
					],
					"text": "overlap $1"
				}
			},
			{
				"box": {
					"id": "obj-901",
					"maxclass": "comment",
					"numinlets": 1,
					"numoutlets": 0,
					"fontname": "Arial",
					"text": "ape.density~",
					"fontsize": 30.0,
					"patching_rect": [
						15.0,
						8.0,
						560.0,
						42.0
					],
					"fontface": 1
				}
			},
			{
				"box": {
					"id": "obj-902",
					"maxclass": "comment",
					"numinlets": 1,
					"numoutlets": 0,
					"fontname": "Arial",
					"text": "Granular synthesis on a sound file. Arguments: the name of the sound buffer~ and the name of the window buffer~ (the grain envelope).\nOutlets: left signal, right signal, number of active grains.",
					"fontsize": 13.0,
					"patching_rect": [
						15.0,
						52.0,
						880.0,
						37.0
					],
					"linecount": 2
				}
			},
			{
				"box": {
					"id": "obj-903",
					"maxclass": "comment",
					"numinlets": 1,
					"numoutlets": 0,
					"fontname": "Arial",
					"text": "Messages",
					"fontsize": 13.0,
					"patching_rect": [
						15.0,
						126.0,
						300.0,
						21.0
					],
					"fontface": 1
				}
			},
			{
				"box": {
					"id": "obj-904",
					"maxclass": "comment",
					"numinlets": 1,
					"numoutlets": 0,
					"fontname": "Arial",
					"text": "interp\n1 = linear",
					"fontsize": 11.0,
					"patching_rect": [
						105.0,
						150.0,
						84.0,
						32.0
					],
					"linecount": 2
				}
			},
			{
				"box": {
					"id": "obj-905",
					"maxclass": "comment",
					"numinlets": 1,
					"numoutlets": 0,
					"fontname": "Arial",
					"text": "overlap\nmax grains, 1-64",
					"fontsize": 11.0,
					"patching_rect": [
						195.0,
						150.0,
						94.0,
						32.0
					],
					"linecount": 2
				}
			},
			{
				"box": {
					"id": "obj-906",
					"maxclass": "comment",
					"numinlets": 1,
					"numoutlets": 0,
					"fontname": "Arial",
					"text": "reverse\n1 = backward",
					"fontsize": 11.0,
					"patching_rect": [
						295.0,
						150.0,
						84.0,
						32.0
					],
					"linecount": 2
				}
			},
			{
				"box": {
					"id": "obj-907",
					"maxclass": "comment",
					"numinlets": 1,
					"numoutlets": 0,
					"fontname": "Arial",
					"text": "setDensity\ngrains per second",
					"fontsize": 11.0,
					"patching_rect": [
						385.0,
						150.0,
						99.0,
						32.0
					],
					"linecount": 2
				}
			},
			{
				"box": {
					"id": "obj-908",
					"maxclass": "comment",
					"numinlets": 1,
					"numoutlets": 0,
					"fontname": "Arial",
					"text": "double-click:\nall the parameters",
					"fontsize": 11.0,
					"patching_rect": [
						782.0,
						298.0,
						130.0,
						32.0
					],
					"linecount": 2
				}
			},
			{
				"box": {
					"id": "obj-909",
					"maxclass": "comment",
					"numinlets": 1,
					"numoutlets": 0,
					"fontname": "Arial",
					"text": "active grains (after \"info\")",
					"fontsize": 11.0,
					"patching_rect": [
						596.0,
						349.0,
						200.0,
						19.0
					]
				}
			},
			{
				"box": {
					"id": "obj-910",
					"maxclass": "comment",
					"numinlets": 1,
					"numoutlets": 0,
					"fontname": "Arial",
					"text": "Buffers",
					"fontsize": 13.0,
					"patching_rect": [
						15.0,
						580.0,
						300.0,
						21.0
					],
					"fontface": 1
				}
			},
			{
				"box": {
					"id": "obj-911",
					"maxclass": "comment",
					"numinlets": 1,
					"numoutlets": 0,
					"fontname": "Arial",
					"text": "the sound to granulate",
					"fontsize": 11.0,
					"patching_rect": [
						180.0,
						638.0,
						220.0,
						19.0
					]
				}
			},
			{
				"box": {
					"id": "obj-912",
					"maxclass": "comment",
					"numinlets": 1,
					"numoutlets": 0,
					"fontname": "Arial",
					"text": "the grain window (Hanning): double-click to see it",
					"fontsize": 11.0,
					"patching_rect": [
						76.0,
						670.0,
						340.0,
						19.0
					]
				}
			},
			{
				"box": {
					"id": "obj-913",
					"maxclass": "comment",
					"numinlets": 1,
					"numoutlets": 0,
					"fontname": "Arial",
					"text": "look at a buffer",
					"fontsize": 11.0,
					"patching_rect": [
						598.0,
						582.0,
						200.0,
						19.0
					]
				}
			}
		],
		"lines": [
			{
				"patchline": {
					"destination": [
						"obj-7",
						0
					],
					"source": [
						"obj-10",
						0
					]
				}
			},
			{
				"patchline": {
					"destination": [
						"obj-19",
						0
					],
					"source": [
						"obj-100",
						0
					]
				}
			},
			{
				"patchline": {
					"destination": [
						"obj-96",
						0
					],
					"source": [
						"obj-101",
						0
					]
				}
			},
			{
				"patchline": {
					"destination": [
						"obj-19",
						0
					],
					"source": [
						"obj-11",
						0
					]
				}
			},
			{
				"patchline": {
					"destination": [
						"obj-19",
						0
					],
					"source": [
						"obj-12",
						0
					]
				}
			},
			{
				"patchline": {
					"destination": [
						"obj-11",
						0
					],
					"source": [
						"obj-13",
						0
					]
				}
			},
			{
				"patchline": {
					"destination": [
						"obj-16",
						0
					],
					"source": [
						"obj-14",
						0
					]
				}
			},
			{
				"patchline": {
					"destination": [
						"obj-19",
						0
					],
					"source": [
						"obj-16",
						0
					]
				}
			},
			{
				"patchline": {
					"destination": [
						"obj-19",
						0
					],
					"source": [
						"obj-17",
						0
					]
				}
			},
			{
				"patchline": {
					"destination": [
						"obj-19",
						0
					],
					"source": [
						"obj-18",
						0
					]
				}
			},
			{
				"patchline": {
					"destination": [
						"obj-28",
						1
					],
					"source": [
						"obj-19",
						2
					]
				}
			},
			{
				"patchline": {
					"destination": [
						"obj-9",
						1
					],
					"source": [
						"obj-19",
						1
					]
				}
			},
			{
				"patchline": {
					"destination": [
						"obj-9",
						0
					],
					"source": [
						"obj-19",
						0
					]
				}
			},
			{
				"patchline": {
					"destination": [
						"obj-17",
						0
					],
					"source": [
						"obj-21",
						0
					]
				}
			},
			{
				"patchline": {
					"destination": [
						"obj-12",
						0
					],
					"source": [
						"obj-22",
						0
					]
				}
			},
			{
				"patchline": {
					"destination": [
						"obj-10",
						0
					],
					"source": [
						"obj-23",
						0
					]
				}
			},
			{
				"patchline": {
					"destination": [
						"obj-18",
						0
					],
					"source": [
						"obj-24",
						0
					]
				}
			},
			{
				"patchline": {
					"destination": [
						"obj-19",
						0
					],
					"source": [
						"obj-3",
						0
					]
				}
			},
			{
				"patchline": {
					"destination": [
						"obj-19",
						0
					],
					"source": [
						"obj-38",
						0
					]
				}
			},
			{
				"patchline": {
					"destination": [
						"obj-38",
						0
					],
					"source": [
						"obj-39",
						0
					]
				}
			},
			{
				"patchline": {
					"destination": [
						"obj-50",
						0
					],
					"source": [
						"obj-49",
						0
					]
				}
			},
			{
				"patchline": {
					"destination": [
						"obj-19",
						0
					],
					"source": [
						"obj-5",
						0
					]
				}
			},
			{
				"patchline": {
					"destination": [
						"obj-19",
						0
					],
					"source": [
						"obj-52",
						0
					]
				}
			},
			{
				"patchline": {
					"destination": [
						"obj-50",
						0
					],
					"source": [
						"obj-6",
						0
					]
				}
			},
			{
				"patchline": {
					"destination": [
						"obj-19",
						0
					],
					"source": [
						"obj-65",
						0
					]
				}
			},
			{
				"patchline": {
					"destination": [
						"obj-19",
						0
					],
					"source": [
						"obj-7",
						0
					]
				}
			},
			{
				"patchline": {
					"destination": [
						"obj-39",
						0
					],
					"source": [
						"obj-71",
						0
					]
				}
			},
			{
				"patchline": {
					"destination": [
						"obj-19",
						0
					],
					"source": [
						"obj-8",
						0
					]
				}
			},
			{
				"patchline": {
					"destination": [
						"obj-1",
						1
					],
					"source": [
						"obj-9",
						1
					]
				}
			},
			{
				"patchline": {
					"destination": [
						"obj-1",
						0
					],
					"order": 1,
					"source": [
						"obj-9",
						0
					]
				}
			},
			{
				"patchline": {
					"destination": [
						"obj-25",
						0
					],
					"order": 0,
					"source": [
						"obj-9",
						0
					]
				}
			},
			{
				"patchline": {
					"destination": [
						"obj-96",
						0
					],
					"source": [
						"obj-99",
						0
					]
				}
			}
		],
		"parameters": {
			"obj-9": [
				"live.gain~",
				"live.gain~",
				0
			],
			"parameterbanks": {},
			"inherited_shortname": 1
		},
		"dependency_cache": [
			{
				"name": "ape.density~.mxo",
				"type": "iLaX"
			}
		],
		"autosave": 0
	}
}