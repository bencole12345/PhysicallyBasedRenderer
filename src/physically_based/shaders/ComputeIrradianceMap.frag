#version 410

in vec2 TexCoords;

uniform sampler2D radianceMap;

out vec4 FragColour;


#define PI 3.1415926535

// Number of alpha values to choose
#define N_ALPHA 30


/**
 * Maps from texture coordinates to spherical coordinates.
 */
vec2 uvToSpherical(vec2 uv)
{
    float u = uv.x;
    float v = uv.y;

    float phi = (2.0 * u - 1.0) * PI;
    float theta = (0.5 - v) * PI;

    return vec2(phi, theta);
}

/**
 * Maps from spherical coordinates to texture coordinates.
 */
vec2 sphericalToUV(vec2 phiTheta)
{
    float phi = mod(phiTheta.x + PI, 2*PI) - PI;
    float theta = mod(phiTheta.y + PI, 2*PI) - PI;

    float u = (phi + PI) / (2.0 * PI);
    float v = 0.5 - theta / PI;

    return vec2(u, v);
}

/**
 * Samples the texture using spherical coordinates.
 */
vec4 sampleTextureSpherical(vec2 spherical)
{
    vec2 uv = sphericalToUV(spherical);
    return texture(radianceMap, uv);
}

void main()
{
    // Map this texture coordinate into spherical coordinates
    vec2 phiThetaCentral = uvToSpherical(TexCoords);
    float phi = phiThetaCentral.x;
    float theta = phiThetaCentral.y;

    /*
     * Explanation: we're in spherical coordinates, and we need a way to loop
     * over/sample from the hemisphere centred around our current position.
     * Simply doing phi += pi, theta += pi/2 will NOT work! (Think about what
     * that looks like near a pole...)
     *
     * What I'm doing here is extracting the localised basis vectors - these are
     * the directions that you would move in if you were to increment phi, theta
     * and r each by some small value respectively. e_phi and e_theta are tangents
     * to the unit sphere, while e_r is the normal.
     *
     * Then, I sample UNIFORMLY from the hemisphere by first varying an angle
     * alpha from 0 to pi/2 -- this is the angle away from e_r -- then, in the inner
     * loop varying an angle beta from 0 to 2pi rotating around the e_r axis. The
     * bigger the value of alpha, the more beta values we need to maintain the
     * property that we're sampling uniformly. If you used the same number of beta
     * values for all alpha values then you'd collect unfairly many samples for
     * small values of alpha. The alternative to choosing a varying number of beta
     * angles would to use a constant number of beta angles but scale each by
     * sin(alpha).
     *
     * (Yes, I could have used cosinal importance sampling here. I did it this way
     * because I implemented it this way first, and I'm satisfied with the results.
     * I also invented this method myself, sadly unlike cosinal importance sampling!)
     */

    // Compute the basis vectors e_phi, e_theta, e_r in Cartesian coordinates.
    // Derivation: write the expression for mapping from spherical polar to Cartesian
    // coordinates then differentiate wrt phi, theta and r respectively
    vec3 e_phi = normalize(vec3(cos(phi)*cos(theta), 0, sin(phi)*cos(theta)));
    vec3 e_theta = vec3(-sin(phi)*sin(theta), cos(theta), cos(phi)*sin(theta));
    vec3 e_r = vec3(sin(phi)*cos(theta), sin(theta), -cos(phi)*cos(theta));

    // Compute the integral
    vec4 result = vec4(0.0);
    for (int i = 0; i < N_ALPHA; i++) {

        // Alpha is the angle between the normal and the point we're currently sampling
        float alpha = float(i)/(N_ALPHA-1) * PI / 2.0;

        // Setting J = 4*i would be "fairer" here, but then you don't get a single sample
        // at the pole, which is the strongest contributor of all. It's a trade-off.
        int J = 4*i + 1;
        for (int j = 0; j < J; j++) {

            // Beta is the angle around the e_r axis.
            float beta = float(j)/J * 2.0 * PI;

            // Use beta to rotate around e_r to get the point on the unit circle on
            // the plane spanned by e_phi and e_theta.
            vec3 on_ePhi_eTheta_circle = cos(beta)*e_phi + sin(beta)*e_theta;

            // Use alpha to sweep down from the e_r axis to our sample point, still
            // in Cartesian coordinates.
            vec3 sampleCartesian = cos(alpha)*e_r + sin(alpha)*on_ePhi_eTheta_circle;

            // Convert this sample point's coordinates back to spherical polar. It is
            // already guaranteed to be a unit vector because it's on the unit sphere,
            // so we don't need an r coordinate.
            float r_xzPlane = sqrt(sampleCartesian.x*sampleCartesian.x + sampleCartesian.z*sampleCartesian.z);
            float samplePhi = atan(sampleCartesian.x, -sampleCartesian.z);
            float sampleTheta = atan(sampleCartesian.y, r_xzPlane);
            vec2 samplePolar = vec2(samplePhi, sampleTheta);

            // Sample the texture
            vec4 thisSample = sampleTextureSpherical(samplePolar);

            // Add to the accumulator
            result += thisSample * cos(alpha);
        }
    }

    // Compensate for the number of samples collected
    int numSamples = N_ALPHA * (2*N_ALPHA - 1);  // arithmetic series :) (1 + 5 + 9 + ...)
    FragColour = PI * result * (1.0 / numSamples);
}
