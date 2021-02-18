#pragma once

void ShowNHMenu(edict_t* ent);
void ShowNHInfoMenu(edict_t* ent, pmenu_t* p);
void ShowNHHelpMenu(edict_t* ent, pmenu_t* p);
void ShowNHHelpMenu2(edict_t* ent, pmenu_t* p);
void ShowNHHelpMenu3(edict_t* ent, pmenu_t* p);
void ShowNHHelpMenu4(edict_t* ent, pmenu_t* p);
void ShowNHHelpMenu5(edict_t* ent, pmenu_t* p);
void ShowNHAdminMenu(edict_t* ent, pmenu_t* p);
void ShowNHMOTD(edict_t* ent, pmenu_t* p);
void ShowNHModelsMenu(edict_t* ent, pmenu_t* p);
void ShowNHCreditsMenu(edict_t* ent, pmenu_t* p);
void ShowNHCreditsMenu2(edict_t* ent, pmenu_t* p);
void ReturnToNHMainMenu(edict_t* ent, pmenu_t* p);
void CloseNHMenu(edict_t* ent, pmenu_t* p);
void EnterGame(edict_t* ent);
void ObserveGame(edict_t* ent, pmenu_t* p);
void ChaseCam(edict_t* ent, pmenu_t* p);
void SetupBindings(edict_t* ent, pmenu_t* p);

