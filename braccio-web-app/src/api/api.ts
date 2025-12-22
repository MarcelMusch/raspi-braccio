const API_URL = import.meta.env.VITE_API_URL + '/api/robot';

export const setJointPositions = async (setJointcommand: { JC: string; T: number; J1: number; J2: number; J3: number; J4: number; J5: number; J6: number }) => {
  const response = await fetch(`${API_URL}/set-joint-positions`, {
    method: 'POST',
    headers: {
      'Content-Type': 'application/json',
    },
    body: JSON.stringify(setJointcommand),
  });
  console.log(JSON.stringify(setJointcommand))
  return response.json();
};

export const getCurrentPosition = async () => {
  const response = await fetch(`${API_URL}/current-position`);
  return response.json();
};

export const goToHomePosition = async () => {
  const response = await fetch(`${API_URL}/home`);
  console.log(API_URL)
  console.log('Home response:', response);
  return response.json();
};

export const getStatus = async () => {
  const response = await fetch(`${API_URL}/status`);
  console.log('Status response:', response);
  return response.json();
};

export const stopRobot = async () => {
  const response = await fetch(`${API_URL}/stop`);
  return response.json();
};

export const resetRobot = async () => {
  const response = await fetch(`${API_URL}/reset`);
  return response.json();
};

export const wave = async () => {
  const response = await fetch(`${API_URL}/wave`);
  return response.json();
};