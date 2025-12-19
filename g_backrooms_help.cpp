#include "g_local.h"

void Help_OpenMainMenu(edict_t* ent, pmenuhnd_t* hnd);
void Help_OpenTools(edict_t* ent, pmenuhnd_t* hnd);
void Help_OpenMonsters(edict_t* ent, pmenuhnd_t* hnd);
void Help_OpenPhenomena(edict_t* ent, pmenuhnd_t* hnd);
void Help_OpenAbilities(edict_t* ent, pmenuhnd_t* hnd);

void Help_Exit(edict_t* ent, pmenuhnd_t* hnd) {
	PMenu_Close(ent);
}

pmenu_t help_sanity[] = {
	{ "*Sanity", PMENU_ALIGN_CENTER, nullptr, "" },
	{ "", PMENU_ALIGN_LEFT,nullptr,""},
	{ "This place doesn't",PMENU_ALIGN_LEFT,nullptr,""},
	{ "make sense. Just being",PMENU_ALIGN_LEFT,nullptr,""},
	{ "here will drive you crazy.",PMENU_ALIGN_LEFT,nullptr,"" },
	{ "Not to mention the", PMENU_ALIGN_LEFT,nullptr,""},
	{ "things in it.", PMENU_ALIGN_LEFT,nullptr,""},
	{ "", PMENU_ALIGN_LEFT,nullptr,""},
	{ "Tread Carefully", PMENU_ALIGN_LEFT,nullptr,""},
	{ "", PMENU_ALIGN_LEFT,nullptr,""},
	{ "Back", PMENU_ALIGN_LEFT,Help_OpenMainMenu,""}
};

void Open_Sanity(edict_t* ent, pmenuhnd_t* hnd) {
	PMenu_Close(ent);
	PMenu_Open(ent, help_sanity, 0, std::size(help_sanity), nullptr, nullptr);
}

pmenu_t help_almond[] = {
	{ "*Almond Water", PMENU_ALIGN_CENTER, nullptr, "" },
	{ "", PMENU_ALIGN_LEFT,nullptr,""},
	{ "A vital survial tool.",PMENU_ALIGN_LEFT,nullptr,""},
	{ "Drinking this will restore", PMENU_ALIGN_LEFT,nullptr,""},
	{ "your sanity to full.", PMENU_ALIGN_LEFT,nullptr,""},
	{ "", PMENU_ALIGN_LEFT,nullptr,""},
	{ "Found in the Powerup Wheel", PMENU_ALIGN_LEFT,nullptr,""},
	{ "", PMENU_ALIGN_LEFT,nullptr,""},
	{ "Back", PMENU_ALIGN_LEFT,Help_OpenTools,""}
};

void Open_Almond(edict_t* ent, pmenuhnd_t* hnd) {
	PMenu_Close(ent);
	PMenu_Open(ent, help_almond, 0, std::size(help_almond), nullptr, nullptr);
}

pmenu_t help_flashlight[] = {
	{ "*Flashlight", PMENU_ALIGN_CENTER, nullptr, "" },
	{ "", PMENU_ALIGN_LEFT,nullptr,""},
	{ "Helps keep the darkness",PMENU_ALIGN_LEFT,nullptr,""},
	{ "from eating your mind.", PMENU_ALIGN_LEFT,nullptr,""},
	{ "Find batteries to keep", PMENU_ALIGN_LEFT,nullptr,""},
	{ "it charged.", PMENU_ALIGN_LEFT,nullptr,""},
	{ "", PMENU_ALIGN_LEFT,nullptr,""},
	{ "Found in the Powerup Wheel", PMENU_ALIGN_LEFT,nullptr,""},
	{ "", PMENU_ALIGN_LEFT,nullptr,""},
	{ "Back", PMENU_ALIGN_LEFT,Help_OpenTools,""}
};

void Open_Flashlight(edict_t* ent, pmenuhnd_t* hnd) {
	PMenu_Close(ent);
	PMenu_Open(ent, help_flashlight, 0, std::size(help_flashlight), nullptr, nullptr);
}

pmenu_t help_alarm[] = {
	{ "*Alarm Clock", PMENU_ALIGN_CENTER, nullptr, "" },
	{ "", PMENU_ALIGN_LEFT,nullptr,""},
	{ "Creates a loud noise,",PMENU_ALIGN_LEFT,nullptr,""},
	{ "Useful to distract", PMENU_ALIGN_LEFT,nullptr,""},
	{ "something with sensitive", PMENU_ALIGN_LEFT,nullptr,""},
	{ "ears.", PMENU_ALIGN_LEFT,nullptr,""},
	{ "", PMENU_ALIGN_LEFT,nullptr,""},
	{ "Found in the Powerup Wheel", PMENU_ALIGN_LEFT,nullptr,""},
	{ "", PMENU_ALIGN_LEFT,nullptr,""},
	{ "Back", PMENU_ALIGN_LEFT,Help_OpenTools,""}
};

void Open_Alarm(edict_t* ent, pmenuhnd_t* hnd) {
	PMenu_Close(ent);
	PMenu_Open(ent, help_alarm, 0, std::size(help_alarm), nullptr, nullptr);
}

pmenu_t help_emf[] = {
	{ "*EMF Reader", PMENU_ALIGN_CENTER, nullptr, "" },
	{ "", PMENU_ALIGN_LEFT,nullptr,""},
	{ "When powered on, this",PMENU_ALIGN_LEFT,nullptr,""},
	{ "will warn you when certain", PMENU_ALIGN_LEFT,nullptr,""},
	{ "creatures get close.", PMENU_ALIGN_LEFT,nullptr,""},
	{ "", PMENU_ALIGN_LEFT,nullptr,""},
	{ "Found in the Powerup Wheel", PMENU_ALIGN_LEFT,nullptr,""},
	{ "", PMENU_ALIGN_LEFT,nullptr,""},
	{ "Back", PMENU_ALIGN_LEFT,Help_OpenTools,""}
};

void Open_EMF(edict_t* ent, pmenuhnd_t* hnd) {
	PMenu_Close(ent);
	PMenu_Open(ent, help_emf, 0, std::size(help_emf), nullptr, nullptr);
}

pmenu_t help_detector[] = {
	{ "*Detector", PMENU_ALIGN_CENTER, nullptr, "" },
	{ "", PMENU_ALIGN_LEFT,nullptr,""},
	{ "An old compass that",PMENU_ALIGN_LEFT,nullptr,""},
	{ "seems to lead towards", PMENU_ALIGN_LEFT,nullptr,""},
	{ "good things.", PMENU_ALIGN_LEFT,nullptr,""},
	{ "", PMENU_ALIGN_LEFT,nullptr,""},
	{ "Found in the Powerup Wheel", PMENU_ALIGN_LEFT,nullptr,""},
	{ "", PMENU_ALIGN_LEFT,nullptr,""},
	{ "Back", PMENU_ALIGN_LEFT,Help_OpenTools,""}
};

void Open_Detector(edict_t* ent, pmenuhnd_t* hnd) {
	PMenu_Close(ent);
	PMenu_Open(ent, help_detector, 0, std::size(help_detector), nullptr, nullptr);
}

pmenu_t help_watcher[] = {
	{ "*The Watcher", PMENU_ALIGN_CENTER, nullptr, "" },
	{ "", PMENU_ALIGN_LEFT,nullptr,""},
	{ "A Strange man who",PMENU_ALIGN_LEFT,nullptr,""},
	{ "just watches. Don't", PMENU_ALIGN_LEFT,nullptr,""},
	{ "look for too long,", PMENU_ALIGN_LEFT,nullptr,""},
	{ "he'll drive you crazy.", PMENU_ALIGN_LEFT,nullptr,""},
	{ "", PMENU_ALIGN_LEFT,nullptr,""},
	{ "Back", PMENU_ALIGN_LEFT,Help_OpenMonsters,""}
};

void Open_Watcher(edict_t* ent, pmenuhnd_t* hnd) {
	PMenu_Close(ent);
	PMenu_Open(ent, help_watcher, 0, std::size(help_watcher), nullptr, nullptr);
}

pmenu_t help_wireframe[] = {
	{ "*Wireframe", PMENU_ALIGN_CENTER, nullptr, "" },
	{ "", PMENU_ALIGN_LEFT,nullptr,""},
	{ "A blind creature, he",PMENU_ALIGN_LEFT,nullptr,""},
	{ "will hear you if your", PMENU_ALIGN_LEFT,nullptr,""},
	{ "not careful.", PMENU_ALIGN_LEFT,nullptr,""},
	{ "", PMENU_ALIGN_LEFT,nullptr,""},
	{ "Back", PMENU_ALIGN_LEFT,Help_OpenMonsters,""}
};

void Open_Wireframe(edict_t* ent, pmenuhnd_t* hnd) {
	PMenu_Close(ent);
	PMenu_Open(ent, help_wireframe, 0, std::size(help_wireframe), nullptr, nullptr);
}

pmenu_t help_phantom[] = {
	{ "*The Phantom", PMENU_ALIGN_CENTER, nullptr, "" },
	{ "", PMENU_ALIGN_LEFT,nullptr,""},
	{ "Don't get too close,",PMENU_ALIGN_LEFT,nullptr,""},
	{ "It may not move, but", PMENU_ALIGN_LEFT,nullptr,""},
	{ "it means you harm.", PMENU_ALIGN_LEFT,nullptr,""},
	{ "", PMENU_ALIGN_LEFT,nullptr,""},
	{ "endlineoptional", PMENU_ALIGN_LEFT,nullptr,""},
	{ "Back", PMENU_ALIGN_LEFT,Help_OpenMonsters,""}
};

void Open_Phantom(edict_t* ent, pmenuhnd_t* hnd) {
	PMenu_Close(ent);
	PMenu_Open(ent, help_phantom, 0, std::size(help_phantom), nullptr, nullptr);
}

pmenu_t help_mannequin[] = {
	{ "*The Mannequin", PMENU_ALIGN_CENTER, nullptr, "" },
	{ "", PMENU_ALIGN_LEFT,nullptr,""},
	{ "Don't take your eyes",PMENU_ALIGN_LEFT,nullptr,""},
	{ "off it, it moves quick.", PMENU_ALIGN_LEFT,nullptr,""},
	{ "Especially violent to", PMENU_ALIGN_LEFT,nullptr,""},
	{ "those who are insane.", PMENU_ALIGN_LEFT,nullptr,""},
	{ "", PMENU_ALIGN_LEFT,nullptr,""},
	{ "Back", PMENU_ALIGN_LEFT,Help_OpenMonsters,""}
};

void Open_mannequin(edict_t* ent, pmenuhnd_t* hnd) {
	PMenu_Close(ent);
	PMenu_Open(ent, help_mannequin, 0, std::size(help_mannequin), nullptr, nullptr);
}

pmenu_t help_snitch[] = {
	{ "*The Snitch", PMENU_ALIGN_CENTER, nullptr, "" },
	{ "", PMENU_ALIGN_LEFT,nullptr,""},
	{ "This monster acts like",PMENU_ALIGN_LEFT,nullptr,""},
	{ "a stationary alarm. If", PMENU_ALIGN_LEFT,nullptr,""},
	{ "it hears you, the Wireframe", PMENU_ALIGN_LEFT,nullptr,""},
	{ "is sure to be close behind.", PMENU_ALIGN_LEFT,nullptr,""},
	{ "", PMENU_ALIGN_LEFT,nullptr,""},
	{ "Back", PMENU_ALIGN_LEFT,Help_OpenMonsters,""}
};

void Open_Snitch(edict_t* ent, pmenuhnd_t* hnd) {
	PMenu_Close(ent);
	PMenu_Open(ent, help_snitch, 0, std::size(help_snitch), nullptr, nullptr);
}

pmenu_t help_sprint[] = {
	{ "*Sprinting", PMENU_ALIGN_CENTER, nullptr, "" },
	{ "", PMENU_ALIGN_LEFT,nullptr,""},
	{ "Useful to get away.",PMENU_ALIGN_LEFT,nullptr,""},
	{ "Drains your stamina quick.", PMENU_ALIGN_LEFT,nullptr,""},
	{ "", PMENU_ALIGN_LEFT,nullptr,""},
	{ "Hold CTRL", PMENU_ALIGN_LEFT,nullptr,""},
	{ "", PMENU_ALIGN_LEFT,nullptr,""},
	{ "Back", PMENU_ALIGN_LEFT,Help_OpenAbilities,""}
};

void Open_Sprint(edict_t* ent, pmenuhnd_t* hnd) {
	PMenu_Close(ent);
	PMenu_Open(ent, help_sprint, 0, std::size(help_sprint), nullptr, nullptr);
}

pmenu_t help_hide[] = {
	{ "*Hiding", PMENU_ALIGN_CENTER, nullptr, "" },
	{ "", PMENU_ALIGN_LEFT,nullptr,""},
	{ "Lockers can be found",PMENU_ALIGN_LEFT,nullptr,""},
	{ "around, hide in them", PMENU_ALIGN_LEFT,nullptr,""},
	{ "to catch a break from", PMENU_ALIGN_LEFT,nullptr,""},
	{ "the creatures.", PMENU_ALIGN_LEFT,nullptr,""},
	{ "They dont tend to move", PMENU_ALIGN_LEFT,nullptr,""},
	{ "when your not around", PMENU_ALIGN_LEFT,nullptr,""},
	{ "however.", PMENU_ALIGN_LEFT,nullptr,""},
	{ "", PMENU_ALIGN_LEFT,nullptr,""},
	{ "Left Click to Enter/Exit", PMENU_ALIGN_LEFT,nullptr,""},
	{ "", PMENU_ALIGN_LEFT,nullptr,""},
	{ "Back", PMENU_ALIGN_LEFT,Help_OpenAbilities,""}
};

void Open_Hide(edict_t* ent, pmenuhnd_t* hnd) {
	PMenu_Close(ent);
	PMenu_Open(ent, help_hide, 0, std::size(help_hide), nullptr, nullptr);
}

pmenu_t help_lean[] = {
	{ "*Leaning", PMENU_ALIGN_CENTER, nullptr, "" },
	{ "", PMENU_ALIGN_LEFT,nullptr,""},
	{ "Look around corners",PMENU_ALIGN_LEFT,nullptr,""},
	{ "without actually walking", PMENU_ALIGN_LEFT,nullptr,""},
	{ "around them.", PMENU_ALIGN_LEFT,nullptr,""},
	{ "", PMENU_ALIGN_LEFT,nullptr,""},
	{ "Hold ALT and A/D", PMENU_ALIGN_LEFT,nullptr,""},
	{ "", PMENU_ALIGN_LEFT,nullptr,""},
	{ "Back", PMENU_ALIGN_LEFT,Help_OpenAbilities,""}
};

void Open_Lean(edict_t* ent, pmenuhnd_t* hnd) {
	PMenu_Close(ent);
	PMenu_Open(ent, help_lean, 0, std::size(help_lean), nullptr, nullptr);
}

pmenu_t help_breath[] = {
	{ "*Holding Your Breath", PMENU_ALIGN_CENTER, nullptr, "" },
	{ "", PMENU_ALIGN_LEFT,nullptr,""},
	{ "This will help you",PMENU_ALIGN_LEFT,nullptr,""},
	{ "dissapear from monsters", PMENU_ALIGN_LEFT,nullptr,""},
	{ "with good ears.", PMENU_ALIGN_LEFT,nullptr,""},
	{ "Drains Stamina.", PMENU_ALIGN_LEFT,nullptr,""},
	{ "", PMENU_ALIGN_LEFT,nullptr,""},
	{ "Hold SHIFT", PMENU_ALIGN_LEFT,nullptr,""},
	{ "", PMENU_ALIGN_LEFT,nullptr,""},
	{ "Back", PMENU_ALIGN_LEFT,Help_OpenAbilities,""}
};

void Open_Breath(edict_t* ent, pmenuhnd_t* hnd) {
	PMenu_Close(ent);
	PMenu_Open(ent, help_breath, 0, std::size(help_breath), nullptr, nullptr);
}

pmenu_t help_eyes[] = {
	{ "*Closing Your Eyes", PMENU_ALIGN_CENTER, nullptr, "" },
	{ "", PMENU_ALIGN_LEFT,nullptr,""},
	{ "Can't stand looking",PMENU_ALIGN_LEFT,nullptr,""},
	{ "at the watcher anymore?", PMENU_ALIGN_LEFT,nullptr,""},
	{ "Then don't.", PMENU_ALIGN_LEFT,nullptr,""},
	{ "", PMENU_ALIGN_LEFT,nullptr,""},
	{ "Hold C", PMENU_ALIGN_LEFT,nullptr,""},
	{ "", PMENU_ALIGN_LEFT,nullptr,""},
	{ "Back", PMENU_ALIGN_LEFT,Help_OpenAbilities,""}
};

void Open_Eyes(edict_t* ent, pmenuhnd_t* hnd) {
	PMenu_Close(ent);
	PMenu_Open(ent, help_eyes, 0, std::size(help_eyes), nullptr, nullptr);
}

pmenu_t help_loop[] = {
	{ "*Infinite Hallways", PMENU_ALIGN_CENTER, nullptr, "" },
	{ "", PMENU_ALIGN_LEFT,nullptr,""},
	{ "Sometimes hallways just.",PMENU_ALIGN_LEFT,nullptr,""},
	{ "wont end.", PMENU_ALIGN_LEFT,nullptr,""},
	{ "Be careful as the looping", PMENU_ALIGN_LEFT,nullptr,""},
	{ "can drain your sanity.", PMENU_ALIGN_LEFT,nullptr,""},
	{ "", PMENU_ALIGN_LEFT,nullptr,""},
	{ "Back", PMENU_ALIGN_LEFT,Help_OpenPhenomena,""}
};

void Open_Loop(edict_t* ent, pmenuhnd_t* hnd) {
	PMenu_Close(ent);
	PMenu_Open(ent, help_loop, 0, std::size(help_loop), nullptr, nullptr);
}

pmenu_t help_exit[] = {
	{ "*Exit Signs", PMENU_ALIGN_CENTER, nullptr, "" },
	{ "", PMENU_ALIGN_LEFT,nullptr,""},
	{ "These things can pop",PMENU_ALIGN_LEFT,nullptr,""},
	{ "up from time to time,", PMENU_ALIGN_LEFT,nullptr,""},
	{ "don't trust them though", PMENU_ALIGN_LEFT,nullptr,""},
	{ "as they dissapear when", PMENU_ALIGN_LEFT,nullptr,""},
	{ "you get too close.", PMENU_ALIGN_LEFT,nullptr,""},
	{ "Drives you crazy.", PMENU_ALIGN_LEFT,nullptr,""},
	{ "", PMENU_ALIGN_LEFT,nullptr,""},
	{ "Back", PMENU_ALIGN_LEFT,Help_OpenPhenomena,""}
};

void Open_Exit(edict_t* ent, pmenuhnd_t* hnd) {
	PMenu_Close(ent);
	PMenu_Open(ent, help_exit, 0, std::size(help_exit), nullptr, nullptr);
}

pmenu_t help_dark[] = {
	{ "*Dark Spots", PMENU_ALIGN_CENTER, nullptr, "" },
	{ "", PMENU_ALIGN_LEFT,nullptr,""},
	{ "Darkness can envelop",PMENU_ALIGN_LEFT,nullptr,""},
	{ "an area, don't go without", PMENU_ALIGN_LEFT,nullptr,""},
	{ "a light, as the darkness", PMENU_ALIGN_LEFT,nullptr,""},
	{ "eats at your mind, plus", PMENU_ALIGN_LEFT,nullptr,""},
	{ "who knows whats lurking.", PMENU_ALIGN_LEFT,nullptr,""},
	{ "", PMENU_ALIGN_LEFT,nullptr,""},
	{ "Back", PMENU_ALIGN_LEFT,Help_OpenPhenomena,""}
};

void Open_Dark(edict_t* ent, pmenuhnd_t* hnd) {
	PMenu_Close(ent);
	PMenu_Open(ent, help_dark, 0, std::size(help_dark), nullptr, nullptr);
}

pmenu_t help_water[] = {
	{ "*Water Jugs", PMENU_ALIGN_CENTER, nullptr, "" },
	{ "", PMENU_ALIGN_LEFT,nullptr,""},
	{ "Large jugs of almond water",PMENU_ALIGN_LEFT,nullptr,""},
	{ "that restore some sanity.", PMENU_ALIGN_LEFT,nullptr,""},
	{ "Shame you can't take it", PMENU_ALIGN_LEFT,nullptr,""},
	{ "with you.", PMENU_ALIGN_LEFT,nullptr,""},
	{ "", PMENU_ALIGN_LEFT,nullptr,""},
	{ "Back", PMENU_ALIGN_LEFT,Help_OpenPhenomena,""}
};

void Open_Water(edict_t* ent, pmenuhnd_t* hnd) {
	PMenu_Close(ent);
	PMenu_Open(ent, help_water, 0, std::size(help_water), nullptr, nullptr);
}

pmenu_t help_calm[] = {
	{ "*Calm Spots", PMENU_ALIGN_CENTER, nullptr, "" },
	{ "", PMENU_ALIGN_LEFT,nullptr,""},
	{ "Some spots just seem to",PMENU_ALIGN_LEFT,nullptr,""},
	{ "make you feel better.", PMENU_ALIGN_LEFT,nullptr,""},
	{ "Findingand staying in", PMENU_ALIGN_LEFT,nullptr,""},
	{ "these spots can help you", PMENU_ALIGN_LEFT,nullptr,""},
	{ "gain sanity.", PMENU_ALIGN_LEFT,nullptr,""},
	{ "", PMENU_ALIGN_LEFT,nullptr,""},
	{ "Back", PMENU_ALIGN_LEFT,Help_OpenPhenomena,""}
};

void Open_Calm(edict_t* ent, pmenuhnd_t* hnd) {
	PMenu_Close(ent);
	PMenu_Open(ent, help_calm, 0, std::size(help_calm), nullptr, nullptr);
}

pmenu_t menu_tools[] = {
	{ "*TOOLS",PMENU_ALIGN_CENTER,nullptr,""},
	{ "", PMENU_ALIGN_LEFT,nullptr,""},
	{ "Almond Water", PMENU_ALIGN_LEFT,Open_Almond,""},
	{ "Flashlight", PMENU_ALIGN_LEFT,Open_Flashlight,""},
	{ "Alarm Clock", PMENU_ALIGN_LEFT,Open_Alarm,""},
	{ "Detector", PMENU_ALIGN_LEFT,Open_Detector,""},
	{ "EMF Detector", PMENU_ALIGN_LEFT,Open_EMF,""},
	{ "", PMENU_ALIGN_LEFT,nullptr,""},
	{ "Back", PMENU_ALIGN_LEFT,Help_OpenMainMenu,""},
};

pmenu_t menu_monsters[] = {
	{ "*MONSTERS",PMENU_ALIGN_CENTER,nullptr,""},
	{ "", PMENU_ALIGN_LEFT,nullptr,""},
	{ "Wireframe", PMENU_ALIGN_LEFT,Open_Wireframe,""},
	{ "Snitch", PMENU_ALIGN_LEFT,Open_Snitch,""},
	{ "Phantom", PMENU_ALIGN_LEFT,Open_Phantom,""},
	{ "Mannequin", PMENU_ALIGN_LEFT,Open_mannequin,""},
	{ "Watcher", PMENU_ALIGN_LEFT,Open_Watcher,""},
	{ "", PMENU_ALIGN_LEFT,nullptr,""},
	{ "Back", PMENU_ALIGN_LEFT,Help_OpenMainMenu,""},
};

pmenu_t menu_phenomena[] = {
	{ "*PHENOMENA",PMENU_ALIGN_CENTER,nullptr,""},
	{ "", PMENU_ALIGN_LEFT,nullptr,""},
	{ "Infinite Hallway", PMENU_ALIGN_LEFT,Open_Loop,""},
	{ "Dark Spots", PMENU_ALIGN_LEFT,Open_Dark,""},
	{ "Water Jugs", PMENU_ALIGN_LEFT,Open_Water,""},
	{ "Calm Spots", PMENU_ALIGN_LEFT,Open_Calm,""},
	{ "Exit Signs", PMENU_ALIGN_LEFT,Open_Exit,""},
	{ "", PMENU_ALIGN_LEFT,nullptr,""},
	{ "Back", PMENU_ALIGN_LEFT,Help_OpenMainMenu,""},
};

pmenu_t menu_abilities[] = {
	{ "*ABILITIES",PMENU_ALIGN_CENTER,nullptr,""},
	{ "", PMENU_ALIGN_LEFT,nullptr,""},
	{ "Sprinting", PMENU_ALIGN_LEFT,Open_Sprint,""},
	{ "Hiding", PMENU_ALIGN_LEFT,Open_Hide,""},
	{ "Leaning", PMENU_ALIGN_LEFT,Open_Lean,""},
	{ "Holding Your Breath", PMENU_ALIGN_LEFT,Open_Breath,""},
	{ "Closing Eyes", PMENU_ALIGN_LEFT,Open_Eyes,""},
	{ "", PMENU_ALIGN_LEFT,nullptr,""},
	{ "Back", PMENU_ALIGN_LEFT,Help_OpenMainMenu,""},
};

void Help_OpenTools(edict_t* ent, pmenuhnd_t* hnd) {
	PMenu_Close(ent);
	PMenu_Open(ent, menu_tools, 2, std::size(menu_tools), nullptr, nullptr);
}

void Help_OpenMonsters(edict_t* ent, pmenuhnd_t* hnd) {
	PMenu_Close(ent);
	PMenu_Open(ent, menu_monsters, 2, std::size(menu_monsters), nullptr, nullptr);
}

void Help_OpenAbilities(edict_t* ent, pmenuhnd_t* hnd) {
	PMenu_Close(ent);
	PMenu_Open(ent, menu_abilities, 2, std::size(menu_abilities), nullptr, nullptr);
}

void Help_OpenPhenomena(edict_t* ent, pmenuhnd_t* hnd) {
	PMenu_Close(ent);
	PMenu_Open(ent, menu_phenomena, 2, std::size(menu_phenomena), nullptr, nullptr);
}


pmenu_t help_main_menu[] = {
	{ "*The Backrooms Compendium", PMENU_ALIGN_CENTER,nullptr,""},
	{ "", PMENU_ALIGN_LEFT,nullptr,""},
	{ "Sanity", PMENU_ALIGN_LEFT,Open_Sanity,""},
	{ "", PMENU_ALIGN_LEFT,nullptr,""},
	{ "Tools", PMENU_ALIGN_LEFT,Help_OpenTools,""},
	{ "", PMENU_ALIGN_LEFT,nullptr,""},
	{ "Monsters", PMENU_ALIGN_LEFT,Help_OpenMonsters,""},
	{ "", PMENU_ALIGN_LEFT,nullptr,""},
	{ "Phenomena", PMENU_ALIGN_LEFT,Help_OpenPhenomena,""},
	{ "", PMENU_ALIGN_LEFT,nullptr,""},
	{ "Abilities", PMENU_ALIGN_LEFT,Help_OpenAbilities,""},
	{ "", PMENU_ALIGN_LEFT,nullptr,""},
	{ "Exit", PMENU_ALIGN_LEFT,Help_Exit,""},
};

void Cmd_OpenHelpMenu(edict_t* ent) {
	if (ent->client->menu) {
		PMenu_Close(ent);
	}

	PMenu_Open(ent, help_main_menu, 2, std::size(help_main_menu), nullptr, nullptr);

	
}

void Help_OpenMainMenu(edict_t* ent, pmenuhnd_t* hnd) {
	Cmd_OpenHelpMenu(ent);
}

