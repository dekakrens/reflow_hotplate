import { zodResolver } from "@hookform/resolvers/zod";
import { useMutation } from "@tanstack/react-query";
import type { AxiosError } from "axios";
import axios from "axios";
import { useEffect, type FC } from "react";
import { useForm } from "react-hook-form";
import { useToast } from "../../../hooks/useToast";
import type { PID } from "../../../types/common";
import { PIDSchema, type UpdatePIDResponse } from "../schema";

interface PIDSettingsProps {
  pid?: PID;
}

const PIDSettings: FC<PIDSettingsProps> = ({ pid }) => {
  const { showSuccessToast, showErrorToast } = useToast();

  const { register, setValue, handleSubmit } = useForm<PID>({
    resolver: zodResolver(PIDSchema),
  });

  useEffect(() => {
    if (pid) {
      setValue("kp", pid.kp);
      setValue("ki", pid.ki);
      setValue("kd", pid.kd);
    }
  }, [pid]);

  const update = useMutation<UpdatePIDResponse, AxiosError, PID>({
    mutationFn: async (pid) =>
      await axios.put("/api/pid", {
        kp: pid.kp,
        ki: pid.ki,
        kd: pid.kd,
      }),
    onSuccess: (res) => showSuccessToast(res.data.message),
    onError: (err) => showErrorToast(String(err.response?.data)),
  });

  const onSubmit = (data: PID) => update.mutate(data);

  return (
    <form
      onSubmit={handleSubmit(onSubmit)}
      className="card card-border bg-base-300 p-3"
    >
      <p>PID Settings</p>

      <div className="my-3 flex flex-col gap-2">
        <div className="flex items-center gap-2">
          <div className="flex w-12 justify-between">
            <p className="col-span-1">Kp</p>
            <p className="col-span-1">:</p>
          </div>

          <input
            type="number"
            className="input flex-auto focus-within:outline-none"
            step={0.1}
            {...register("kp", { valueAsNumber: true })}
          />
        </div>

        <div className="flex items-center gap-2">
          <div className="flex w-12 justify-between">
            <p className="col-span-1">Ki</p>
            <p className="col-span-1">:</p>
          </div>

          <input
            type="number"
            className="input flex-auto focus-within:outline-none"
            step={0.0001}
            {...register("ki", { valueAsNumber: true })}
          />
        </div>

        <div className="flex items-center gap-2">
          <div className="flex w-12 justify-between">
            <p className="col-span-1">Kd</p>
            <p className="col-span-1">:</p>
          </div>

          <input
            type="number"
            className="input flex-auto focus-within:outline-none"
            step={0.1}
            {...register("kd", { valueAsNumber: true })}
          />
        </div>
      </div>

      <div className="flex justify-end">
        <button type="submit" className="btn btn-warning btn-soft">
          Update
        </button>
      </div>
    </form>
  );
};

export default PIDSettings;
