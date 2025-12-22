// AirwindowsUI by Chris Johnson
// Initial seed code for the meter created by Paul Walker on 8/23/21.
// From then on all this wild stuff is Chris :)
//and huge support from Sudara and Pamplejuce!
#include "AirwindowsUI.h"

void AirwindowsMeter::paint(juce::Graphics &g)
{
    float vS = displayHeight/200.0f; // short for vScale: everything * this
    g.fillAll(juce::Colours::black);
    //blank screen before doing anything, unless our draw covers the whole display anyway
    //this is probably quick and optimized
    g.setColour(juce::Colours::darkgrey);
    g.fillRect(0,  (int)(60.0f*vS), getWidth(),1); // -6dB markings
    g.fillRect(0, (int)(101.02*vS), getWidth(),1); //-12dB markings
    g.fillRect(0, (int)(130.02f*vS), getWidth(),1); //-18dB markings
    g.fillRect(0, (int)(150.2f*vS), getWidth(),1); //-24dB markings
    g.fillRect(0, (int)(164.9f*vS), getWidth(),1); //-30dB markings
    g.fillRect(0, (int)(175.2f*vS), getWidth(),1); //-36dB markings
    g.fillRect(0, (int)(182.5f*vS), getWidth(),1); //-42dB markings
    
    for (unsigned long count = 0; count < fmin(displayWidth,5150); ++count) //count through all the points in the array
    {
        g.setColour(juce::Colours::black);
        float psDotSizeL = 5.0f*dataA[count]*dataA[count];
        float psDotSizeR = 5.0f*dataB[count]*dataB[count];
        float peakL = dataC[count] * 200.0f;
        float peakR = dataD[count] * 200.0f;
        float slewL = sqrt(dataE[count])*300.0f;
        float slewR = sqrt(dataF[count])*300.0f;
        float meterZeroL = (sqrt(dataG[count])*6.0f)-6.0f;
        if (meterZeroL > 192.0f) meterZeroL = 192.0f;
        float meterZeroR = (sqrt(dataH[count])*6.0f)-6.0f;
        if (meterZeroR > 192.0f) meterZeroR = 192.0f;
        
        //begin draw dots on meters L
        if (peakL > 196.0f) {
            g.setColour(juce::Colour(255, 0, 0));
            g.fillRect((float)count, ((200.0f-fmin((peakL-196.0f),196.0f))*vS), 1.0f, (fmin((peakL-196.0f),196.0f)*vS));
        }//peak is clipping!
        g.setColour(juce::Colour::fromFloatRGBA(0.0f, 0.0f, psDotSizeL, 1.0f));
        g.fillRect((float)count, ((fmin((sqrt(meterZeroL)*14.142f)+26.0f,199.0f))*vS), ((dataA[count]*1.618f)+1.0f), fmax(8.0f*dataA[count]*vS,1.0f));
        //zero cross subs
        if (peakL > 1.0f && peakL < 196.0f) { //peak isn't clipping, but is not literally zero so there's something here to work with
            float departure = (slewL-peakL)*fabs(slewL-peakL);
            float hue = 0.25f+(departure*0.000073f);
            float saturation = 1.0f-fmax(hue-0.47f,0.0f);
            hue = fmin(hue,0.47f);
            if (hue < 0.0f) hue = fmax(hue-1.0f,0.69f);
            if (slewL > 196.0f) g.setColour(juce::Colours::white); //slew clip is rendered as brightest white
            else g.setColour(juce::Colour::fromHSV(hue, saturation, 1.0f, 1.0f));
            g.fillRect((float)count, ((200.0f - peakL)*vS), ((dataA[count]*1.618f)+1.0f), (fmax(fmax(psDotSizeL*vS,1.0f), dataC[count]*4.0f*vS)));
        } //end draw dots on meters L
        //begin draw dots on meters R
        if (peakR > 196.0f) {
            g.setColour(juce::Colour(255, 0, 0));
            g.fillRect((float)count, ((200.0f-fmin((peakR-196.0f),196.0f))*vS), 1.0f, (fmin((peakR-196.0f),196.0f)*vS));
        }//peak is clipping!
        g.setColour(juce::Colour::fromFloatRGBA(0.0f, 0.0f, psDotSizeR, 1.0f));
        g.fillRect((float)count, ((fmin((sqrt(meterZeroR)*14.142f)+26.0f,199.0f))*vS), ((dataB[count]*1.618f)+1.0f), fmax(8.0f*dataB[count]*vS,1.0f));
        //zero cross subs
        if (peakR > 1.0f && peakR < 196.0f) { //peak isn't clipping, but is not literally zero so there's something here to work with
            float departure = (slewR-peakR)*fabs(slewR-peakR);
            float hue = 0.25f+(departure*0.000073f);
            float saturation = 1.0f-fmax(hue-0.47f,0.0f);
            hue = fmin(hue,0.47f);
            if (hue < 0.0f) hue = fmax(hue-1.0f,0.69f);
            if (slewR > 196.0f) g.setColour(juce::Colours::white); //slew clip is rendered as brightest white
            else g.setColour(juce::Colour::fromHSV(hue, saturation, 1.0f, 1.0f));
            g.fillRect((float)count, ((200.0f - peakR)*vS), ((dataB[count]*1.618f)+1.0f), fmax(fmax(psDotSizeR*vS,1.0f), dataD[count]*4.0f*vS));
        } //end draw dots on meters R
    }
    g.setColour(juce::Colours::lightgrey);
    g.fillRect((int)dataPosition, 0, 1, (int)(200.0f*vS)); //the moving line
    float scaleFont = sqrt(vS*50.0f)*1.6f;
    if (scaleFont > 8.0f) {
        g.setColour(juce::Colour::fromHSV(0.0f, 0.0f, fmin(scaleFont-8.0f,0.7f), 1.0f));
        g.setFont(scaleFont);
        g.drawText("-6 dB", 0, (int)(60.0f*vS)-7, displayWidth-6, (int)scaleFont, juce::Justification::bottomRight);
        g.drawText("-12 dB", 0, (int)(101.02f*vS)-7, displayWidth-6, (int)scaleFont, juce::Justification::bottomRight);
        g.drawText("-18 dB", 0, (int)(130.02f*vS)-7, displayWidth-6, (int)scaleFont, juce::Justification::bottomRight);
        g.drawText("-24 dB", 0, (int)(150.2f*vS)-7, displayWidth-6, (int)scaleFont, juce::Justification::bottomRight);
        g.drawText("-30 dB", 0, (int)(164.9f*vS)-7, displayWidth-6, (int)scaleFont, juce::Justification::bottomRight);
        g.drawText("-36 dB", 0, (int)(175.2f*vS)-7, displayWidth-6, (int)scaleFont, juce::Justification::bottomRight);
        g.drawText("-42 dB", 0, (int)(182.5f*vS)-7, displayWidth-6, (int)scaleFont, juce::Justification::bottomRight);
        //right edge dB markings
        g.drawText("6.8k", 6, (int)(60.0f*vS)-7, displayWidth, (int)scaleFont, juce::Justification::bottomLeft);
        g.drawText("720", 6, (int)(101.02f*vS)-7, displayWidth, (int)scaleFont, juce::Justification::bottomLeft);
        g.drawText("230", 6, (int)(130.02f*vS)-7, displayWidth, (int)scaleFont, juce::Justification::bottomLeft);
        g.drawText("120", 6, (int)(150.2f*vS)-7, displayWidth, (int)scaleFont, juce::Justification::bottomLeft);
        g.drawText("80", 6, (int)(164.9f*vS)-7, displayWidth, (int)scaleFont, juce::Justification::bottomLeft);
        g.drawText("60", 6, (int)(175.2f*vS)-7, displayWidth, (int)scaleFont, juce::Justification::bottomLeft);
        g.drawText("50", 6, (int)(182.5f*vS)-7, displayWidth, (int)scaleFont, juce::Justification::bottomLeft);
        //left edge zero cross markings
    }
}
