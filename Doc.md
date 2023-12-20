The basic initial goal of this project is it's meant to sort of be a spiritual successor to a game called '7 days to die' (7d2d) in the survival horror sandbox genre mix with voxel mechanics.  Obviously not wanting it to be a direct rip but still try and do justice for the niche it currently fills.  To explain that game a bit.  It's older updates used to constantly send a little or maybe even hordes off zeds (zombies) at you as you gather resources to grow a base and equipment and generally just things you need to survive longer.  Save for recent updates it was a very brutal and horror driven piece of art.  It's been losing it's spirit with time am suspecting as a result of just not having the architecture backend to support the crazy ideas they've had in the past.  So maybe they've been desperate.  Feel free to ask me more or really just ask anyone that's been around a long time that's hosting large PVP servers.  The game's  been around on steam since about 2013 and continues to get updates, and conceptually it's very interesting.  Building is even realistic due to their structural integrity system so you couldn't easily cheese with floating blocks.  And the PVP scene was rather complex and engaging for it.  There will be some example videos posted somewhare to help better visualize the game in play.  Without further delay here's the meat of this design doc.

### Alright so to get started lets set some serious goals to orient the architecture around.
*Support for 100+ players per server<p>
*Support for 50-100 zeds per player (yup to a max of 5k-10k)<p>
  -Zeds roam, may be spawned in buildings, never made into "sleapers" though.<p>
*Support for 10 animals per player (up to 1k)<p>
*Support for at least 10 ally NPC colonists  per player (up to 1k)<p>
*Support for 12kx12k+ worlds (potentially bigger)<p>
*Support for vehicles with smooth chunk loading<p>
*Steady balanced AI and pathfinding, regardless of server load<p>
*Minimal desync!<p>
*Prediction and semi lockstep based networking<p>
*Minimal state sharing<p>
*Clients have no control over any game logic aside their own input states<p>
*Data oriented and data driven code and mod support<p>
*NO FLOAT REGISTERS!<p>
	Read: how-can-i-perform-a-deterministic-physics-simulation <p>
*ECS for all game logic<p>
	Wiew: Overwatch-Gameplay-Architecture-and-Networking <p>
*Made in C++ (std 14 latest) with ALL SOURCE SHARED PUBLICALLY!<p>
*Crossplay support and easy porting across different platforms and rendering backends<p>

### Why did i start with architecture goals?
The honest reason is that the main hinderence of 7d2d which currently is the only leading game in this genre mix am aware of, is undeniably the architecture and thus the runtimes and code/mechanical stability.  Though there are design problems as well and they are slightly of consequence to the above problems.  We can never encapsulate the true spirit of this game wtihout first building it a foundation to be proud of.

### Now for the actual design goals.
This is a "survival horror" "sandbox" game not a pointlessly grindy RPG.  This is also partially as a consequence a tower defense game.  Yes you heard me right.  Why tower defense?  Base defense!  That thing which used to be such a common mechanic many alphas ago which is now dwindled into  nothing.  The devs couldn't achieve their prevous goals and wanted more content and for players to move around more often, so the consequence is they've made the game into a pointless RPG grind fest as of alpha 20 (honeslty this started gradually way before).  So all new invented mechanics must be in service to the mentioned goals.  Integrity is hard but always worth striving for.  Far as the theme being a little different the new mechanics are going to involve colonist NPCs that you can find and ally with.  Essentially you can roam with them or guide them back to your base to help defend and/or process materials or even help harvest materials/farm.  They should require constant upkeep so being smart about how you design the colony will be essential to helping it thrive.  There's also designs in the works related to using light mechanics to play a much bigger part in this game, from effecting NPCs in interesting ways to being required in order to really loot at night or even progress at all in some areas.  With tradeoffs for using it.  Turning gamma way up to compensate shouldn't be an added feature.  Everyone should have the same visual experience as well despite wanting to target lower end hardware.  We'll fix exploits related as they come up.


### And some PR goals.
*When this project goes commerecial hopefully there's a way to do open book keeping without annoying employees.<p>
*For those giving direct assistance, we must give credit where due in some reasonable way.<p>
*We are to put the user experience way above any selfish individual or collective goals.<p>
*Mistakes will not be actively hidden, everyone fucks up.<p>
*We will not bend a knee to cancel culture or political correctness.  The game is what matters.  Target audience is mature meaning 18+.