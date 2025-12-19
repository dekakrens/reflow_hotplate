import { zodResolver } from "@hookform/resolvers/zod";
import { useMutation } from "@tanstack/react-query";
import axios, { type AxiosError } from "axios";
import { useForm } from "react-hook-form";
import { useToast } from "../../hooks/useToast";
import { heaterSchema, type HeaterResponse, type HeaterType } from "./schema";

const Heater = () => {
  const { showSuccessToast } = useToast();

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

  const start = useMutation<HeaterResponse, AxiosError, number>({
    mutationFn: async (temperature) =>
      await axios.post("/api/heater-start", {
        temperature,
      }),
    onSuccess: (res) => showSuccessToast(res.data.message),
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
          Start
        </button>
      </div>
    </form>
  );
};

export default Heater;
