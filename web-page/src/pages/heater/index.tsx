import { zodResolver } from "@hookform/resolvers/zod";
import { useMutation } from "@tanstack/react-query";
import axios, { type AxiosError, type AxiosResponse } from "axios";
import { useState } from "react";
import { useForm } from "react-hook-form";
import type { MachineState } from "../../types/common";
import { heaterSchema, type HeaterResponse, type HeaterType } from "./schema";

const Heater = () => {
  const [state, setState] = useState<MachineState>("STOPPED");

  const {
    register,
    handleSubmit,
    formState: { errors },
  } = useForm<HeaterType>({
    resolver: zodResolver(heaterSchema),
    mode: "onSubmit",
    defaultValues: {
      temperature: undefined,
    },
  });

  const start = useMutation<AxiosResponse<HeaterResponse>, AxiosError, number>({
    mutationFn: async (temperature) =>
      await axios.post("/heater-start", {
        temperature,
      }),
    onSuccess: (data) => setState(data.data.state),
  });

  const onSubmit = (value: HeaterType) => start.mutate(value.temperature);

  return (
    <form onSubmit={handleSubmit(onSubmit)}>
      <fieldset className="fieldset">
        <legend className="fieldset-legend">Temperature</legend>

        <div className="flex gap-2">
          <input
            {...register("temperature", { valueAsNumber: true })}
            type="number"
            className="input w-full focus-within:outline-none"
            step="1"
          />
        </div>

        {errors.temperature?.message && (
          <p className="label text-error">{errors.temperature.message}</p>
        )}
      </fieldset>

      <div className="mt-2 flex justify-end gap-2">
        <button
          type="submit"
          className="btn btn-error btn-soft focus-within:outline-none"
        >
          {state === "STARTED" ? "Stop" : "Start"}
        </button>
      </div>
    </form>
  );
};

export default Heater;
