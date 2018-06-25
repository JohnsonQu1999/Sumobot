# Sumobot
Programmer: Johnson Qu
---

This Arduino program contains the basic methods & strategies for a successful Sumobot. The program is split into 4 main sections.

1.) Search

2.) Attack

3.) Evade

4.) Starting Strategy

---

1.) The Search methods are a critical part to this Sumobot as it is this method that locates the enemy. There are 2 ways to accomplish this.
  a.) ActiveSearch
    This technique involves actively looking for the enemy bot, usually by quickly turning a tight circle until the enemy bot is found. This technique is favoured if you only have sensors point in front of your Sumobot.
  b.) RandomSearch
    This technique simply causes the Sumobot to drive until the ring's limits, then turning left/right, until an enemy bot is found. This technique is favoured if you have sensors all around your Sumobot.

2.) The Attack method is the core of your Sumobot. This method simply moves forwards as long as the enemy bot is in front of it. Numerous other methods enable the Sumobot to keep track of the enemy even if it moves from side to side.

3.) Ideally Evade() would never be used because the Sumobot should always know where the enemy bot is. Nevertheless if our Sumobot loses sight of the enemy bot this method causes the Sumobot to back up and turn. This is because we are assuming that the enemy has gone to our side.

4.) The Starting Strategy is entirely up to you to create. This is useful because having only 1 starting strategy makes it easy for your opponent to anticipate what your Sumobot will do.


For more details please see comments within the code.
