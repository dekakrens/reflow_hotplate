import * as z from "zod";
import type { ReflowProfile, ReflowProfileName } from "../../types/common";

export const reflowSchema = z.object({
  profile: z.string().min(1, { error: "Please select a profile" }),
});

export type ReflowType = z.infer<typeof reflowSchema>;

export type ReflowResponse = {
  data: {
    profiles: ReflowProfileName;
  };
};

export type ProfilesResponse = {
  data: {
    profiles: ReflowProfile[];
  };
};
