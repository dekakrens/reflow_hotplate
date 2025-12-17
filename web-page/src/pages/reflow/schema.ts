import type { MachineState, ReflowProfileName } from "../../types/common";
import * as z from "zod";

export const reflowSchema = z.object({
  profile: z.string().min(1, { error: "Please select a profile" }),
});

export type ReflowType = z.infer<typeof reflowSchema>;

export type ReflowResponse = {
  state: MachineState;
  profile: ReflowProfileName;
};
