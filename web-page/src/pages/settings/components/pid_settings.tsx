import { useEffect, type FC } from "react";
import type { PID } from "../../../types/common";
import { useForm } from "react-hook-form";
import { PIDSchema } from "../schema";
import { zodResolver } from "@hookform/resolvers/zod";

interface PIDSettingsProps {
  pid?: PID;
}

const PIDSettings: FC<PIDSettingsProps> = ({ pid }) => {
  const { register, setValue } = useForm<PID>({
    resolver: zodResolver(PIDSchema),
  });

  useEffect(() => {
    if (pid) {
      setValue("kp", pid.kp);
      setValue("ki", pid.ki);
      setValue("kd", pid.kd);
    }
  }, [pid]);

  return (
    <form className="card card-border bg-base-300 p-3">
      <p>PID Settings</p>

      <div className="my-3 flex flex-col gap-2">
        <div className="flex items-center gap-2">
          <div className="flex w-12 justify-between">
            <p className="col-span-1">P</p>
            <p className="col-span-1">:</p>
          </div>

          <input
            type="number"
            className="input flex-auto focus-within:outline-none"
            {...register("kp")}
          />
        </div>

        <div className="flex items-center gap-2">
          <div className="flex w-12 justify-between">
            <p className="col-span-1">I</p>
            <p className="col-span-1">:</p>
          </div>

          <input
            type="number"
            className="input flex-auto focus-within:outline-none"
            {...register("ki")}
          />
        </div>

        <div className="flex items-center gap-2">
          <div className="flex w-12 justify-between">
            <p className="col-span-1">D</p>
            <p className="col-span-1">:</p>
          </div>

          <input
            type="number"
            className="input flex-auto focus-within:outline-none"
            {...register("kd")}
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
