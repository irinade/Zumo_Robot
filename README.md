# Zumo_Robot

For our Robotic class we were tasked to right the code of a Zumo bot to make it fight in the Arena against other Zumo bots, and push them off the ring.

![Our zumo bot triumphing against a fierce enemy](https://api.projects.cri-paris.org/api/projects/MV7w6CGO/images/61b23f8c536538097a751c4a)

Zumo bots have lots of already implemented captors such as border detection (to not get out of the ring which is delimited by a white border), accelerometer (is your bot being pushed by another bot), a buzzer (for victory song which of course we decided was gonna be the Mario song in our case) ...

We wanted to give our robot sight. We followed [this](https://penturalabs.wordpress.com/2014/04/09/it-can-see-giving-your-bot-sight/) tutorial and welded to the zumo bot wires connected to an ultrasonic sensor.

Our bot has the ability to detect white borders on a black ground and turn to drive away from the border to stay in the ring (so he could basically drive on a highway for sure), he can detect objects using his ultrasonic sensor in a way that if he sees any robot passing 8 cm in front of him he will accelerate to push him off the line, and he will sing his Mario battle song the all time. Isn't he amazing !!
