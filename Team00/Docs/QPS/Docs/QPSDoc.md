
![QPS Overview Diagram](../Diagrams/QPSOverview.png)
![QPS Sequence Diagram](../Diagrams/QPSSequence.png)

## **SE Principles**


1. Single Responsibility Principle
<br/>
We used SRP in our design as each component is responsible for a single purpose only. 

2. Open Close Principle
<br/>
Each Query Object is a class on its own. Hence, it is extensible as we can add more Query Object types, and the Query Object class is closed to modifications

3. Don’t Repeat Yourself
<br/>
We abstracted all our code into methods, removing the need to repeat code

4. Keep it Simple Stupid
<br/>
We abstracted each class to handle a single responsibility by keeping it simple

## **Design Patterns**

1. Facade Pattern
<br/>
We used facade pattern between the PKB and the QPS which aligns with Single Responsibility Principle 

2. Chain of Responibility
<br/>
We used the chain of responsiblity behavioural pattern. Each Class handles it's responsiblity and passes the output to the next Class, mirroring this design pattern.
