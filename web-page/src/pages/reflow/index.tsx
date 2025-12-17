import { zodResolver } from "@hookform/resolvers/zod";
import { useMutation, useQuery } from "@tanstack/react-query";
import axios from "axios";
import { useEffect, useState } from "react";
import { useForm } from "react-hook-form";
import type { MachineState, ReflowProfileName } from "../../types/common";
import { reflowSchema, type ReflowResponse, type ReflowType } from "./schema";

const Reflow = () => {
  const [state, setState] = useState<MachineState>("STOPPED");

  const { data } = useQuery<ReflowResponse>({
    queryKey: ["REFLOW_VALUE"],
    queryFn: async () => await axios.get("/REFLOW"),
  });

  useEffect(() => {
    if (data) {
      setState(data.state);
    }
  }, [data]);

  const {
    handleSubmit,
    setValue,
    formState: { errors },
  } = useForm<ReflowType>({
    resolver: zodResolver(reflowSchema),
    mode: "onSubmit",
  });

  const setProfile = useMutation({
    mutationFn: async (profile: ReflowProfileName) =>
      await axios.post("/heater", { profile }),
  });

  const start = useMutation({
    mutationFn: async () =>
      await axios.post("/reflow-start", {
        state: state === "STARTED" ? "STOPPED" : "STARTED",
      }),
  });

  const onSubmit = (value: ReflowType) =>
    setProfile.mutate(value.profile as ReflowProfileName);

  return (
    <form onSubmit={handleSubmit(onSubmit)}>
      <fieldset className="fieldset">
        <legend className="fieldset-legend">Select Profile</legend>

        <select
          onChange={(e) => setValue("profile", e.currentTarget.value)}
          className="select select-bordered w-full focus-within:outline-none"
        >
          <option disabled selected aria-disabled>
            Select profile
          </option>
          <option value="LEAD">Lead</option>
          <option value="LEAD-FREE">Lead-Free</option>
        </select>

        {errors.profile?.message && (
          <p className="label text-error">{errors.profile.message}</p>
        )}
      </fieldset>

      <div className="mt-2 flex justify-end">
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

export default Reflow;
