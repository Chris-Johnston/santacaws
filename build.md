# Build Instructions for your very own Santa CAWs

Congratulations on delivery of your own Santa CAWs DIY kit. This took a while
longer than anticipated to get out of the door, so thanks for waiting.

## Stuff that's included

- 1 Santa CAWs circuitboard w/ ATTiny404 pre-soldered and pre-flashed on the back
- 3 "Shiny Trinket" circuitboards, a bottle cap, button and thread, and paperclips
  - You get all three because you are saving me the effort of soldering them
- 6 1206 size 1k resistors. The silkscreen on them says '1001'
- 1 1206 size capacitor (no silkscreen)
- 1 2032 battery holder
  - No battery included because USPS is weird about shipping batteries, sorry!
- 1 slide switch
- 1 pushbutton
- 8 1206 size LEDs
- At least 3 female right angle pin headers
- At least 9 male right angle pin headers

### Why is the microcontroller pre-soldered and pre-flashed?

So while you are going to put this all together yourself, I don't expect most of you to have a breakout for the ATTiny's UPDI pins. I figured then it made sense to pre-flash everything.

If you feel inclined to hack on this once the board is assembled, this is what you'll need to reprogram the board. The code is also in this repo.

## Stuff you need

- A soldering iron with a relatively fine tip
- Some solder
- Solder wick
- Tweezers
- Diagonal cutters
- Not required but recommended: cotton swaps and isopropyl alcohol to clean up the flux

# Assembly

This is the fun part, hopefully. If you need a primer on soldering surface mount parts, [the instructions from the crow of judgement board still applies](https://github.com/chris-johnston/crowofjudgement?tab=readme-ov-file#assembly), and [this guide from Digikey seems helpful too](https://www.digikey.com/en/maker/tutorials/2023/how-to-solder-surface-mount-components-using-common-tools).

If you haven't soldered surface mount parts before, [this type surface mount practice kit on amazon seems useful](https://www.amazon.com/Gikfun-Welding-Practice-Soldering-Training/dp/B00VWB8F8K).

## 1. Start on the back 1206 parts

I'm recommending you start on the back surface mount parts because these are the easiest ones to start on if you don't have a ton of surface mount soldering experience.

Solder parts R1 through R6, and C1. None of these parts need to face a particular way, they just have to be stuck on.

**Do not** solder the buttons on yet, we'll come back to this side.

## 2. Front surface mount LEDs

Hopefully the parts on the back went well, this part is a bit harder, and requires a bit more care so the front doesn't look scorched or anything.

I don't know what I was thinking when I laid out the parts on this board, because the polarity of the LEDs is all over the place.

TODO: need to make a diagram for how these are laid out

https://i.sstatic.net/1TP3Z.gif

## 3. The battery holder

The battery holder goes through the pins in the center of the board. The negative (squared) side of the holder should point in the direction fo the UPDI pins, so towards the left from the back.

Soldering this part is straightforward. Sometimes the positive pin in the battery holder can fall out, but it can just as easily go back in.

The pins on the end of the battery holder are sharp, I would strongly recommend trimming them with a diagonal cutter.

## 4. The power switch and pattern button

Next solder on the power switch and the button. Not sure if I have anything to add.

## 5. The female pin headers

You have an option here, and I would recommend you "test fit" this first.

The female pin headers should point out the front side of the main board, and can either point up or down. Pointing up will obscure more of the main board, pointing down will obscure less.

Once you've decided, solder these on and try to make sure they are in at a 90 degree angle.

## 6. The accessory boards

I may or may not have included parts to solder all of the accessory boards. They all should have the same pinout, and the same schematic.

TODO: need to include a diagram for how the LEDs go on

The placement of the LEDs is kinda tricky, because they all point in different ways.

## 7. Add battery and test

Once you are done, you should just be able to insert a battery (face side points out), switch it on, and cycle through patterns.

If any LEDs do not light up, I would recommend checking:

- the pins on the microcontroller, to ensure none are shorted or not fully soldered
- the resistors
- the leds

Reflowing the solder is a good way to check this again.

You know where to find me if things continue to not work. Have fun!