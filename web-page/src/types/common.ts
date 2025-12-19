export type MachineState = "STARTED" | "STOPPED" | "DONE" | "ERROR";

export type PID = {
  kp: number;
  ki: number;
  kd: number;
};

export type ReflowProfile = {
  name: string;
  preheatTemp: number;
  soakTemp: number;
  reflowTemp: number;
  preheatTime: number;
  soakTime: number;
  reflowTime: number;
};

export type ReflowProfileName = "LEAD" | "LEAD-FREE";
