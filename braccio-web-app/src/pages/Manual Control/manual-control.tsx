import React, { useState } from 'react';
import { IonButton, IonContent, IonHeader, IonTitle, IonToolbar, IonText, IonModal, } from '@ionic/react';
import { getCurrentPosition, goToHomePosition, getStatus, stopRobot, resetRobot, wave, } from '../../api/api';
import JointControl from '../../components/JointControl';

const ManualControl: React.FC = () => {
    const [output, setOutput] = useState<string>('');
    const [isModalOpen, setIsModalOpen] = useState(false);

    const handleApiResponse = async (apiCall: () => Promise<any>) => {
        try {
            const data = await apiCall();
            setOutput(JSON.stringify(data, null, 2));
            return data;
        } catch (error) {
            setOutput(`Error: ${error}`);
        }
    };

    return (
        <IonContent className="ion-padding">
            <IonButton expand="block" onClick={() => handleApiResponse(getCurrentPosition)}>
                Get Current Position
            </IonButton>
            <IonButton expand="block" onClick={() => handleApiResponse(goToHomePosition)}>
                Go to Home Position
            </IonButton>
            <IonButton expand="block" onClick={() => handleApiResponse(getStatus)}>
                Get Status
            </IonButton>
            <IonButton expand="block" onClick={() => handleApiResponse(stopRobot)}>
                Stop Robot
            </IonButton>
            <IonButton expand="block" onClick={() => handleApiResponse(resetRobot)}>
                Reset Robot
            </IonButton>
            <IonButton expand="block" onClick={() => handleApiResponse(wave)}>
                Wave
            </IonButton>
            <IonButton expand="block" onClick={() => setIsModalOpen(true)}>
                Control Joints
            </IonButton>
            <IonModal isOpen={isModalOpen} onDidDismiss={() => setIsModalOpen(false)}>
                <IonHeader>
                    <IonToolbar>
                        <IonTitle>Control Joints</IonTitle>
                        <IonButton slot="end" onClick={() => setIsModalOpen(false)}>Close</IonButton>
                    </IonToolbar>
                </IonHeader>
                <IonContent>
                    <JointControl />
                </IonContent>
            </IonModal>
            <IonText>
                <pre>{output}</pre>
            </IonText>
        </IonContent>
    );
};

export default ManualControl;