# The problem: locating facilities
Try and relate problem to something in people's lives.

    - Choosing a vacation spot? Not exactly right
    - Planning a high school reunion? (deciding where to meet) Maybe

Where we build things _matters_. But how do we decide where to build things?

Two basic metrics: efficiency and fairness.
The two most well-known problems solve for these metrics (sum star and max radius, respectively--we'll explain the names in a second).
The sum star problem adds up all the costs of assigning customers to facilities and tries to minimize that sum.
The max radius tries to minimize the most expensive cost that any customer has to bear.

## The actors: facilities and customers
There are two actors in our script: facilities and customers. There is a _cost_ associated with assigning a customer to a facility--think about the cost of driving. As distance increases, cost goes up. A customer is assigned to their closest facility--because who wants to drive any extra miles just because some bureaucrat says no? No way, pal. If we keep track of the distances that each customer has to travel, then we can try to make things more efficient or more fair. 

How do we keep track of distances? For our purposes, we're going to keep track of distances at each facility. Think about middle managers from different offices coming to a board meeting with statistics about their branches. What statistics do they bring?
As it turns out, there are a whole bunch of different things they could track, but let's just talk about two for now.

## The "star"?
The "star" of a facility is the sum of all of the distances that its customers have to travel. So if Jim drives 15 miles to work and Sally drives 10, and they're the only two people who work at their tiny little office, then the "star" of their office is 25 miles.

Why is it called the star? Let's draw it visually. So we have some customers (in red) and a facility (in blue). We draw some lines connecting the customers to their facility, and label each line with a number. 
[include a graphic]


## The "radius"?
The "radius" of a facility is the longest distance that any of its customers have to travel. Returning to Jim and Sally, since Jim drives the farthest, the radius of their office is 15 miles.

Why is it called the radius? Let's draw it again.
[include a graphic]
That doesn't look like much. It's just a line. But hey, I paid a little attention in high school geometry. Don't you normally hear "radius" and "circle" in the same sentence? Let's use the line we've got to draw a circle.

[include a graphic]
Now it's making more sense. The farthest distance--the "radius"--defines a circle that contains all of the other customers.




You with me so far? Good. Okay, so we've got a list of stars or radii. What do we do with it? Now we need to decide what's important to us. Now imagine the CEO gathering up all the different statistics that his branch managers brought him. What does she do with them? For right now, let's talk about __two__ different things she could do: find the max, or find the sum.

## The max
Here we worry about the biggest of the measures. For radius, for instance, we won't worry about every single radius. We'll pick out who has to travel the farthest and then try to minimize _only their costs_. At first, this doesn't make much sense. How can reducing one person's cost make the situation more fair?

Let's think it through. Jim has to drive an extra 10 miles to work. Poor Jim. Let's move his office to try and make his commute shorter.
Can we? Yes! We do! Jim now has to drive 7 extra miles. But what about Sally? When we moved the office, Sally didn't have to drive any farther, but now she's the person who has to drive the farthest. Let's try and move the office again. So we do, but now Erik has the longest commute, so . . ..

And so on and so on, until eventually the situation can't be improved. If you think about the problem in this way, you can see that even though we focus on one person at a time, we're actually improving the situations of lots of people.

## The sum
In this case, we worry about _all_ of the costs equally. We add them all up and then try to minimize that number.

The original problem names should be making sense now. The "sum star" problem minimizes all of the costs; the "max radius" problem minimizes only the worst cost. 


At this point, you might be thinking to yourself, "Okay, you've got stars and radii, and maxes and sums. You can combine those to solve problems. But so far you've combined them in only two ways? Aren't there more ways?"

###YES!

Check out this handy-dandy table:

[include the max/sum star/radius table]

That's the whole point of our research! We feel that these other combinations have been neglected. In fact, while people have solved many different kinds of facility location problems, nobody (that we can find) has published a solution to either the "sum radius" or "max star" problems.

## Why?
These problems are _hard_, man. Like really hard. Like in our initial investigations, we wrote a program to solve a small max star problem exactly. We let it run for over 24 hours before we gave up and stopped it. The sum star and max radius problems can be solved much, much faster.

Our new problems also have different goals. Max star is kind of about fairness, but it's a new definition. Sum radius, to be frank, is kind of useless. You'll see that later on. It gives you some weeeeeeeird facility locations that don't make much sense. But maybe somebody can tweak it and find some use for it.



Let's recap. We've got these really cool new problems that also happen to be really hard to solve. How can we solve them?

Using imaginary bees, of course!
