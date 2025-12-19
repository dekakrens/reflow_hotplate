import * as z from "zod";

export const heaterSchema = z.object({
  temperature: z
    .number()
    .min(100, { error: "The min temperature is 100" })
    .max(450, { error: "The max temperature is 450" }),
});

export type HeaterType = z.infer<typeof heaterSchema>;

export type HeaterResponse = {
  data: {
    status: string;
    message: string;
    temperature: number;
  };
};
