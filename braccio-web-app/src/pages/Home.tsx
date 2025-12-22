import { IonContent, IonHeader, IonPage, IonTitle, IonToolbar } from '@ionic/react';
import ManualControl from './Manual Control/manual-control';

const Home: React.FC = () => {
  return (
    <IonPage>
      <IonHeader>
        <IonToolbar>
          <IonTitle>Manual Braccio Control</IonTitle>
        </IonToolbar>
      </IonHeader>
      <IonContent className="ion-padding">
        <ManualControl />
      </IonContent>
    </IonPage>
  );
};

export default Home;