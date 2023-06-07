# Simulation rules
This document describes set of rules used to run this simulation.

Simulation rules are updated when previous set of rules is implemented.
Newer version contains sophisticated logic compared to the previous versions.

**Version of the document: v0.1**

## Units and Parties
Unit is an atomic entity. Each unit has number of parameters and actions to do.
On the map units are represented as a Party, even if they are moving alone.
Party can include units of the same type.

### Humans
- Power: 1 per unit
- Party size is unlimited
- Action: Establish new city

### Heroes
- Power: 5 per unit
- Party: can have only 4 units
- Action: Attack Orcs party
- Action: Attack Orcs Tribe
- Action: Protect the city

### Orcs
- Power: 2 per unit
- Party size is unlimited
- Action: Attack City
- Action: Attack Heroes party
- Action: Attack Humans party

## Settlements
### Human city
#### Values
- Food
- Gold
- Population

#### Actions
- Generate 3 food each day
- Generate 1 gold each day
- Generate 1 Human each 3 days
- Create Party of citizens to establish new city: 20 food
- Create Hero: 10 Gold and 1 Human Citizen

### Orcs Tribe
#### Values
- Population
#### Actions
- Generate 1 Orcs each day
- Create Orcs party of a random size (5-20) each 5 days

## Resources
- Food - waste to create Cities
- Gold - waste to create Heroes

## Fights
Fights are handled by comparing Power values among opposing sides. 
The side that has more Power winning a fight.

- Power of the party = Sum Power of all units in this party
- Power of the city = Sum Power of all units in the settlement + Defence level of city

There are different fight scenarios to handle:
### Party vs. Party
Losing party is destroyed

### Party vs. City
If city lost fight
- Half of all the resources are destroyed
- Created full-packed party of heroes
If Party lost
- Party destroyed

### Party vs. Tribe
If tribe lost fight
- Tribe destroyed
If Party lost
- Party destroyed