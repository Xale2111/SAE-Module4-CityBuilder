City builder made with CMake and SFML. Assets from firstyear students of the SAE Geneva


Not implemented (lack of time) : Npc doesn't have the hunger system, so the cateen is useless for now. 
Wanted and how I would have implemented it :
Npcs has a timer of hunger
when timer hits 0, they need to go the canteen to eat (eating costs 75 food)
A maximum of 10 Npcs can be at the canteen at the same time (when they eat, they stay there for around 45 seconds)
A node in the behaviour tree needs to be added to check If the npc is working, when he finishes his task, he can go to the canteen
The Npc will then need to :
 - Find the closest Canteen (BFS like for the closest resource)
 - Check if there's space for him
 - Yes ? -> block that space, A* to the canteen (if there's no path, starts looking for a canteen again)
 - No ? Look for another canteen
 - While he doesn't eat in a canteen, he loses health. When he finished eating, health is full
 - Once he has eaten, reset hunger timer, go back home and start working again
 - If an NPC health drops to 0, he dies
 - If an Npc can access a canteen and is starving, a popup tells the player and the npc blinks in red

Building management system (not added, lack of time):
If the player loses an Npc, he can destroy the house or add a new npc to the house. Houses can have up to 5 npcs in them. The cost of an npc is half of the cost of the house. Destroying a house gives you back 1/3 of the cost. 
