# pihole_mon
Dirty simple light pihole stats using the API.

This experimental project came into existence because of the need for a way to monitor the status of the piHole DNS resolver instance that is currently running in my network, at a glance.
Running a firefox tab non-stop increases the load of the  CPU to absurd levels in about a week and I got tired of re-opening the pihole page in a new tab every couple of days/weeks.
Thus this monstruosity was born.
All that was needed was minimal info to alert if I needed to login to actually investigate activity, as light on resources as possible (there aleady is a mini-LCD connected at all times).

It uses SDL2 to render graphics,
http-get to get data from the piHole server from Stephen Mathieson (https://github.com/clibs/http-get.c),
cJSON to parse the data and 
adapted code to plot a graph in SDL from Bertrand Martel(https://github.com/bertrandmartel/plot-sdl).

The fonts are free on the web.
The code is an instable mess and needs lots of work for anything serious. Feel free to turn it into whatever you like.
Absolutely no warranty or claim for anything.
License could be WtfPL at best (you will find remnants of that in the code).

WARNING!
You need to change the IP of http_get calls in the code! Put the IP of your piHole!
Also:
In order to get the graph to show (not just the summary at the top), you need an authenticated call to the pihole API.
That means replace the auth token on line 12 of file pi-data.c with your personal token. Info on how to generate/get this
can be found at https://chrisbergeron.com/2018/06/21/pihole_api/ .

![screenshot](https://github.com/atomicbombermaniac/pihole_mon/blob/main/pihole-mon.png?raw=true)
