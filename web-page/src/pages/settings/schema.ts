import * as z from "zod";
import type { PID, ReflowProfile } from "../../types/common";

export const PIDSchema = z.object({
  kp: z.number(),
  ki: z.number(),
  kd: z.number(),
});

export const ProfileSchema = z.object({
  name: z.string().min(1, { error: "Profile name is required" }),
  preheatTemp: z.number(),
  soakTemp: z.number(),
  reflowTemp: z.number(),
  preheatTime: z.number(),
  soakTime: z.number(),
  reflowTime: z.number(),
});

export type SettingResponse = {
  data: {
    pid: PID;
    profiles: ReflowProfile[];
  };
};

export type UpdatePIDResponse = {
  data: {
    status: string;
    message: string;
  };
};
