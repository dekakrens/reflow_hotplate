import { zodResolver } from "@hookform/resolvers/zod";
import { useMutation, useQuery } from "@tanstack/react-query";
import axios from "axios";
import { useEffect, useState } from "react";
import { useForm } from "react-hook-form";
import type { MachineState } from "../../types/common";
import { heaterSchema, type HeaterResponse, type HeaterType } from "./schema";

const Heater = () => {
  const [state, setState] = useState<MachineState>("STOPPED");

  const { data } = useQuery<HeaterResponse>({
    queryKey: ["HEATER_VALUE"],
    queryFn: async () => await axios.get("/heater"),
  });

  useEffect(() => {
    if (data) {
      setState(data.state);
    }
  }, [data]);

  const {
    register,
    handleSubmit,
    formState: { errors },
  } = useForm<HeaterType>({
    resolver: zodResolver(heaterSchema),
    mode: "onSubmit",
  });

  const setTemperature = useMutation({
    mutationFn: async (temperature: number) =>
      await axios.post("/heater", { temperature }),
  });

  const start = useMutation({
    mutationFn: async () =>
      await axios.post("/heater-start", {
        state: state === "STARTED" ? "STOPPED" : "STARTED",
      }),
  });

  const onSubmit = (value: HeaterType) =>
    setTemperature.mutate(value.temperature);

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
          className="btn btn-warning btn-soft focus-within:outline-none"
        >
          Set Temperature
        </button>

        <button
          type="button"
          onClick={() => start.mutate()}
          className="btn btn-error btn-soft focus-within:outline-none"
        >
          {state === "STARTED" ? "Stop" : "Start"}
        </button>
      </div>
    </form>
  );
};

export default Heater;
