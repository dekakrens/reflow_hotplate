import * as z from "zod";
import type { PID, ReflowProfile } from "../../types/common";

export const PIDSchema = z.object({
  kp: z.number(),
  ki: z.number(),
  kd: z.number(),
});

export type SettingResponse = {
  pid: PID;
  profiles: ReflowProfile[];
};
