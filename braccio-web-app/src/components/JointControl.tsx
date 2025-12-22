import React, { useEffect, useState, useRef } from 'react';
import { IonCard, IonItem, IonLabel, IonRange, IonSpinner } from '@ionic/react';
import { getCurrentPosition, setJointPositions } from '../api/api';

const JointControl: React.FC = () => {
  const [jointPositions, setJointPositionsState] = useState<Record<string, number>>({
    j1: 0,
    j2: 0,
    j3: 0,
    j4: 0,
    j5: 0,
    j6: 0,
  });
  const [isMoving, setIsMoving] = useState(false); // State to track if the robot is moving

  const isFetching = useRef(false); // Ref to track fetch state

  const jointLimits: Record<string, { min: number; max: number }> = {
    j1: { min: 0, max: 180 },
    j2: { min: 15, max: 165 },
    j3: { min: 0, max: 180 },
    j4: { min: 0, max: 180 },
    j5: { min: 0, max: 180 },
    j6: { min: 10, max: 73 },
  };

  const fetchCurrentPositions = async () => {
    if (isFetching.current) return; // Prevent overlapping fetches
    isFetching.current = true;

    try {
      const ans = await getCurrentPosition();
      if (ans.success)
          setJointPositionsState(ans.position);
    } catch (error) {
      console.error('Failed to fetch current positions:', error);
    } finally {
      isFetching.current = false;
    }
  };

  useEffect(() => {
    fetchCurrentPositions(); // Fetch positions when the dialog is opened
  }, []);

  const handleSliderChange = async (joint: string, value: number) => {
    const updatedPositions = { ...jointPositions, [joint]: value };
    setJointPositionsState(updatedPositions);
    setIsMoving(true); // Disable inputs while waiting for response

    try {
      // Prepare the API payload
      const jointControlPayload = {
        JC: Object.keys(jointPositions)
          .map((j) => (j === joint ? "1" : "0"))
          .join(""),
        T: 20, // Step-Delay time in milliseconds
        ...Object.fromEntries(
          Object.entries(jointPositions).map(([key, val]) => [key.toUpperCase(), key === joint ? value : val])
        ),
      };
      
      console.log('Joint Control Payload:', jointControlPayload);
      const response = await setJointPositions(jointControlPayload);

      if (response.success) {
        console.log(`Updated joint ${joint} to position ${value}`);
      } else {
        console.error(`Failed to update joint ${joint}, fetching current positions.`);
        await fetchCurrentPositions(); // Fetch positions if the update failed
      }
    } catch (error) {
      console.error(`Failed to update joint ${joint}:`, error);
      await fetchCurrentPositions(); // Fetch positions in case of an error
    } finally {
      setTimeout(() => setIsMoving(false), 200); // Add a 0.2-second delay before re-enabling inputs
    }
  };

  return (
    <IonCard>
      {Object.entries(jointPositions).map(([joint, value]) => (
        <IonItem
          key={joint}
          style={{
            display: 'flex',
            alignItems: 'center',
            gap: '10px',
            opacity: isMoving ? 0.5 : 1, // Grey out controls if moving
          }}
        >
          <IonLabel style={{ whiteSpace: 'nowrap' }}>{joint.toUpperCase()}:</IonLabel>
          <IonLabel style={{ whiteSpace: 'nowrap' }}>Min: {jointLimits[joint].min}</IonLabel>
          <IonRange
            min={jointLimits[joint].min}
            max={jointLimits[joint].max}
            value={value}
            onIonChange={(e) => handleSliderChange(joint, e.detail.value as number)}
            disabled={isMoving} // Disable input if moving
          />
          <IonLabel style={{ whiteSpace: 'nowrap' }}>Max: {jointLimits[joint].max}</IonLabel>
          <IonLabel style={{ whiteSpace: 'nowrap' }}>Current: {value}</IonLabel>
        </IonItem>
      ))}
      {isMoving && (
        <div style={{ textAlign: 'center', color: 'gray', marginTop: '10px' }}>
          <IonSpinner name="crescent" />
          <p>Robot is moving...</p>
        </div>
      )}
    </IonCard>
  );
};

export default JointControl;