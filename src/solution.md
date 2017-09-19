# Bees? Bees!

Alright I've been talking a mean game about bees, but it's time for me to quit with the clickbait and actually get down to business.

The fancy name for what I'm talking about is __Particle Swarm Optimization__, or PSO.
PSO is an algorithm that's been around for about 20 years that's supposedly based on how birds flock. I don't know anything about that, but I like bees better so I always say bees instead of birds. There's something about our little insect mothers that I just love. Sue me.

So how does this mumbo-jumbo actually work? Let's break it down.

Leslie has a couple flowers scatter throughout her yard. Since the flower nectar is extra-sweet due to the toxic runoff from a nearby candy factory, the local bees are dying to get their antennae on these bad boys. Unfortunately, due to that same toxic runoff, the bees can tell only how close they are to a flower, but not in which direction it is. They can shout back in forth in their little bee voices, but that and flower-sense is about all they've got.

Let's start by dropping some bees randomly in Leslie's yard. Immediately each bee knows how close it is to a flower, but it can't tell in which direction the flower is. The first thing the bees do is shout back and forth to determine who is the closest to a flower. Turns out its Dave. Bless his little bee soul. The other bees start thinking, "Okay, Dave is close to a flower, so maybe let's move a little in his direction. But not too much. Dave sometimes forget to wear bee deoderant." And so everybody moves a little towards Dave, but also a little randomly since they're bees and moving in a straight line is hard, and then they stop again.

Now they shout back and forth in their little bee voices to determine who is the closest, but they also do something else. They _remember_ the closest they've ever been to a flower, and they remember where in Leslie's yard that was. Now the bees take that under consideration as well before they move. So, there's three components to figuring out where a bee is going to move next:

    1. Where is the bee now?
    2. Where is the best place the bee has ever been?
    3. Where is the current bee who is closest to a flower?

Each of the components pulls the bee in a different direction. The bee moves a little randomly, a little in the direction of its personal best, and a little in the direction of the whole swarm's best, and badabing badaboom--it ends up in a new position, hopefully closer to a flower than it was before.

Repeat this a whole bunch with a whole lot of bees, and eventually you're pretty much guaranteed to hit a flower.


If you're a math buff, the equation for PSO is surprisingly simple:
[include the equation]
where c1, c2, and c3 represent different parameters; xt, xp, and gt represent different positions


The thing about PSO is that mathematicians and computer scientists don't like "pretty much" guaranteed. They like "100% money-back guarantees". Absolute certainty. The way bees fly is kind of random, and there's a genuine chance that they may never find a flower. It's also tough to explain why the bee method works. We know that it does, but I haven't seen any rigorous mathematical proofs. Finally, the method itself has to make some assumptions before it can get started. How much should a bee move randomly, how much should it trust its personal best, and how much should it follow the best in the swarm? We can assign numbers to these questions, and we don't have any foolproof way for choosing these numbers. It's pretty much a crapshoot. Also, how do you decide when to stop your swarm? There a couple different ways, but there's no foolproof way to decide. There's always the risk that if you let it run just a liiiiiittle bit longer you would have found a better answer.

(later on I'll talk about how I picked the numbers that this website uses. Feel free to [jump ahead] and come back if you want)

That said, PSO has remained a common and powerful way of solving complicated problems. It's common because it's fairly straightforward and surprisingly easy to write. It's powerful because, well, we keep using it and we know that it keeps working.

Now that you've got a handle on my imaginary bees, let's move on. How did I use imaginary bees to solve the problems we talked about in the last section?

Hint: I didn't.

# Jumping Frogs
Okay I _sort of_ lied to you, but not totally. The method we use __IS__ based on the whole bee thing. We just had to shake it up a little bit in order for it to work.

As it turns out, the whole "bee" metaphor doesn't exactly apply to facilities. Somebody much smarter than me proved years ago that, under certain conditions, you don't need to consider every possible place that you might put a facility. You don't need to worry about an inch to the left or a mile to the right--you can trust that the places you're looking at are the best possible places. We're going to go ahead and assume that these conditions hold for now. Why? Because it makes my life easier.

So "bees" don't make much sense anymore. A bee can move three inches to the left or half an inch to the right. A facility is either at Platform 9 or Platform 10. There is no Platform 9 3/4 here (unfortunately).

Instead of bees, it helps to think about frogs. Imagine different possible facility locations as lily pads. A frog is always on a lily pad. There's only one frog per lily pad. The frogs move by jumping from pad to pad. Only one frog moves at a time, and it's smart enough to always jump to an empty lily pad.

Where do the "flowers" exist in this version of the metaphor? Uhh . . . they don't. This is where the metaphor starts breaking down. Our problem is about locating multiple facilities, right? If each frog represents a single facility, then it isn't the frogs that find "nectar", its the whole _pond_ of frogs that does. Does that make any sense?

No, it doesn't to me either. That's the stupid thing about math. Stuff that works sometimes makes absolutely no sense whatsoever.

## this metaphor doesn't really work
## previously the bee's location determined fitness, but now the combination of frogs determines fitness
## that's tough to explain
## and then how do I explain that each group of frogs now represents a single bee, so we've got a whole lot of frog-groups flying around?
## . . . I need a better metaphor
## Or maybe I can keep this one, but just use it better
## Ooh, or add a new element? Maybe we put the frogs on the back of cosmic turtles, and the TURTLE is now a bee
## then the turtles come together and discuss how to rearrange their frogs
## maybe. I'll think about it


Let's try jumping (ribbet) to a more concrete example.

We're trying to locate two new Dairy Queens in town. Customers are in blue; potential DQ sites are in green.
[ include a graphic ]
Let's drop two DQs onto potential sites at random. The new facilities are in red.
[ include a graphic ]
I now have my own personal map of where I think the DQs should go. Now imagine that there are 99 other people who just did the same thing.
Using some math, we can determine which of these 100 potential sites is the "best".
Turns out its this one.
[ include graphic ]
Note that this combination isn't necessarily the BEST EVER. It's just the best that anybody has right now. Somebody could have found a better solution before, and someone may find a better solution in the future. It's just the best we've got right now.

Now I want to build a new map. How do I do it?
I start by picking three maps: the one I have right now, the best one I've ever had, and the best that anybody currently has.
I go through each map. I flip a coin. If heads, I move a single facility randomly. If tails, I do nothing.
Once I've changed (or not changed) all of the maps, I pick the best among them.
(Note that the best among them is not necessarily better than where I am now. It could be that all of my random changes created WORSE maps!
For reasons that are tough to explain [ but are explained here ], this is actually a good thing as long as it doesn't happen too often.)

Now I have a new map. The 99 other people also went through the same process and have 99 new maps. We find the new global best and run through the whole process again. And again. And about 997 more times, until we get tired and decide to stop.

That's how it works. Did it make any sense? If you're still confused, maybe watching it in action will help.

[ write an interactive version of DPSO that allows you to step through and introspect the swarm ]