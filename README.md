CSCI 164 Final Project - Factory Simulation
Matt Doherty, Nicholas Fong, Julia Marcondes, Christina Matera

Project Description:
    A factory holds 7 machines, each of which can be one of two varieties of machine. The first variety is a machine with a high throughput and quick production speed, but also more likely to break down and be out of service. When this first machine type breaks down, it is slow to repair, so it is out of commission for a long time, and the cost of repairs are higher.
    The second variety is a machine with a low throughput and slow production speed, but also less likely to break down and be out of service. When this second machine type breaks down, it is quick to    repair, so it is only out of commission for a short period of time, with a lower cost of repairs.
    There is only one repair crew for each class of machine so only one machine of each class can be repaired at a time.
    We will run a simulation to predict the most efficient combination of the first and second varieties in our factory of seven machines. We will run this simulation over a long period of time and will be able to report the frequency of extreme events (ex. three days out of year, all systems down), and also the overall rate of production for each combination of machines.

We need 2 Weibull distributions:
    1 for how long machine is running
    1 for how long it took to repair (high alpha, low beta)

State Variables:
    We plan to record the information for the machines by creating two classes: one class for each of the two varieties of machines. The 7 machines will be recorded as combinations of instances of these two classes. The following state variable will be added as a member variable to each machine.
    Number of machines that are broken of each type
    State of each machine (either broken or working)
    Each machine will have a state variable associated with it, which will always be checked before the machine is able to operate. Whenever the state is recorded as “broken,” we will take it out of commission. The repair subroutine will address any broken machines (one at a time) and the machines will come back into production as soon as the given time of repair has elapsed.
    Repair Queue (one for each type of machine)
    Seeing as how we can only repair one fast and one slow machine at a time, we will need two repair team queues so we can keep track of how many machines are currently being repaired, or in line to be repaired.

Distributions:
    Rate of production for each machine
        Both the slow and fast machines will have a given throughput. This rate of production will be associated with each machine based on which variety it is (slow or fast). Each time we simulate a different combination of machine varieties, we will change the combination of machines, but during each individual simulation, this variable will not change. It is a fundamental property of the two different machines.
    Rate of repair for each machine
        Slow machines and fast machines will have their own distributions to represent the time that each machine spent in the shop. At the end of each simulation, we will collect the average time that each type of machine spent out of the factory and analyze how much of a loss the factory experienced during the machines’ absence. 
    Repair cost for each machine
        Each machine will have their own distribution dependent on the severity of their breakdowns. Fast machines will generally have costly repairs and slow machines will have reasonable prices. During each simulation, we will collect the costs for each machine and calculate the best net worth (total profit - total costs of repairs).

Counter Variables:
    Total products made
        During each simulation, we will keep a tally of the total products made. This will represent the total income that the machines produced. This is a major key in analyzing the most efficient combination of machines.
    Cost of repairs
        After each repair, we will collect the cost of each machine and sum them all. At the end of each simulation, we will subtract the total cost of repairs from the total income of each simulation to calculate the best profit. 
    End of period profit (total products made - cost of repair)
        The end-of-period profit will be the deciding factor to determine the best combination of fast and slow machines. We are able to calculate this because we will keep track of all products made and the total cost of repairs.
    Total wait time for repairing machines
        This will help determine which machine is most efficient overall. Although slow machines produce less product, it may be more efficient because the wait time for fast machines may be much larger than the slow. This variable will account for this. While the end of period profit may seem like the universal scale of efficiency, it is worth noting if the machines have a extreme amount of down time that could impact operations. This could result in a situation that might have the best overall end profit, but having these extreme periods of low activity could make the situation suboptimal.
    Time of break down
        We will keep track of the time of break down of each machine so can return the broken machine to the factory once it has been repaired. 
    Time machine returned from repair
        This will be the time that the machine broke down plus the time it took for its repair. At this time, we will flip the said machine’s state from broken to working and return it to the factory.

System Parameters:
    Number of fast machines and slow machines
        We will simulate each run with every combination of the fast and slow machines. At the end of each simulation, we will average the total products made, how many machines broke and for how long, and total costs and profits. 

Event List:
    A machine breaks down:
        Add machine to the appropriate repair queue
            - Increase that queue by 1.
                - If the queue is 1 then generate a repair time, update counter variables
                - If queue is >=2 then simply increase queue number 
        Adjust production rate
    A machine is repaired:
        Adjust production rate
        Reduce queue by 1. 
            If new queue is >0 then generate a repair time, update counter variables
        Once repaired, we will schedule when it’ll break down again
    A product is made (We might not need this if the rate of products being made is super high since the amount we truncate from just doing rate x time is negligible)

Search Strategy:
    Perform many trials each combination of n fast machines and (7-n) slow machines with  0 <= n <= 7, averaging the results to find the combination that optimizes profit and efficiency, by taking into account:
        Cost of each machine
        Cost of repairs
        Revenue from products (just a function of how many products are made)
        Machine down time (Only indirectly affects profits so this might not be needed, but it might be relevant if we want a more consistent factory)
