import { zodResolver } from "@hookform/resolvers/zod";
import { useMutation, useQuery } from "@tanstack/react-query";
import axios, { type AxiosError, type AxiosResponse } from "axios";
import { useForm } from "react-hook-form";
import type { ReflowProfileName } from "../../types/common";
import {
  reflowSchema,
  type ProfilesResponse,
  type ReflowResponse,
  type ReflowType,
} from "./schema";

const Reflow = () => {
  const { data } = useQuery<ProfilesResponse>({
    queryKey: ["PROFILES"],
    queryFn: async () => await axios.get("/api/profiles"),
  });

  const {
    handleSubmit,
    setValue,
    formState: { errors },
  } = useForm<ReflowType>({
    resolver: zodResolver(reflowSchema),
    mode: "onSubmit",
    defaultValues: {
      profile: "",
    },
  });

  const start = useMutation<AxiosResponse<ReflowResponse>, AxiosError, string>({
    mutationFn: async () =>
      await axios.post("/api/reflow-start", {
        state: "START",
      }),
  });

  const onSubmit = (value: ReflowType) =>
    start.mutate(value.profile as ReflowProfileName);

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

          {data?.data.profiles &&
            data?.data.profiles.map((profile) => (
              <option key={profile.name} value={profile.name}>
                {profile.name}
              </option>
            ))}
        </select>

        {errors.profile?.message && (
          <p className="label text-error">{errors.profile.message}</p>
        )}
      </fieldset>

      <div className="mt-2 flex justify-end">
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

export default Reflow;
