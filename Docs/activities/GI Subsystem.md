
The Game Instance Subsystem controls creating and maintaining Activities

### Activity Instance

What needs to be instanced:

*  Stage Service (needs a world pointer)
*  Activity Tracker (needs a world pointer)

Create these instanced elements when we enter into the stage

Instance must contain:

*  Activity Asset
*  Gameplay Tags
*  Players In It
*  Current Stage
*  Current Stage Services
*  Current Objective Trackers

When a tracker updates, can set it's result to Success, Failure, or Don't Care

When a tracker updates, it reports back to it's instance that it's changed.  The Instance then checks all trackers if they complete the objective



Activities Instances are a UObject that contains a pointer to the Activity asset, instanced pointers to each Stage and Objective Tracker

It holds a list of shared pointers to Activity Instances.  Each instance maintains a pointer to the activity, it's current Stage, a pointer to it's current blackboard